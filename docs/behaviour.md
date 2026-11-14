# Behaviour

This chapter models the behaviour of the system with **finite state machines (FSMs)**. It defines the state machines of each
device, how they interact with each other, and why they are designed as **synchronous Moore machines**.

## 1. Modelling approach

### 1.1 Discrete and continuous components

The system contains both kinds of components described in the theory:
- **discrete components** evolve through atomic changes (a door is closed or opening, a request is granted or denied). They are modelled with FSMs;
- **continuous components** evolve smoothly: the flight kinematics (speed, acceleration, altitude, distance) and the battery charge.
They are not FSMs: they are modelled as simple equations, integrated with a fixed time step dt, e.g. v(t + dt) = v(t) + a · dt.

### 1.2 Extended FSMs

All machines are **extended FSMs (EFSMs)**: besides the discrete state, they use variables (timers, counters, the door angle,
the current reqId, the drone being inside or not)
. This keeps the number of discrete states small. Timeouts, for example, are counters of reactions, not additional states.

### 1.3 Globally asynchronous, locally synchronous

The two devices run on two different microcontrollers, with no shared clock. **Their FSMs are therefore concurrent and asynchronous
with respect to each other**: they coordinate only by exchanging messages, and a message produced by one machine is an input event for the other (section 5).

On each device, instead, the machines are synchronous (time-triggered): they react periodically, with a fixed period.
Time is part of the specification: timeouts, the gradual movement of the door, sensor sampling, the time step of the flight simulation.
This scheme is known as **GALS (globally asynchronous, locally synchronous)**.

Several synchronous FSMs run concurrently on the same device, one per task, each with its own period (section 6).

## 2. Moore machines

### 2.1 The choice

All FSMs in the system are designed as **Moore machines**. In the diagrams, the outputs are written inside each state, and the transitions are labelled only with their guards.

Outputs are of two kinds:
- **level outputs**, held for the whole time the machine is in a state: the door command, the servo angle, the LED pattern, the active flight dynamics, the state published on MQTT;
- **messages**, emitted once when the machine *enters* a state (entry actions, as in UML statecharts): a message is a
discrete event, so sending it at every reaction while in the state would be wrong.


### 2.2 Why Moore and not Mealy

1. **The published state is the output.** Each device publishes its current state on a retained MQTT topic. With a Moore 
machine that state fully describes what the device is doing, so other components can rely on it. For example, the hangar
does not need a separate *"I have landed"* message: it observes that the drone's state has become `AT_REST`. The protocol
gets simpler, and the dashboard shows exactly what drives the outputs.
2. **Idempotent outputs after a failure.** Since every output is bound to a state, it can be reproduced at any time from
the state alone.
3. **Simpler implementation and tests.** Each state is a class with an `onEnter()` method (messages) and an output function (levels),
following the _State pattern_. A unit test checks a single fact: *"after reaching state X, the outputs are Y"*.

**Trade-offs accepted.** A Moore machine reacts to an input one reaction later than the equivalent Mealy machine, and sometime
needs a few extra transient states (e.g. `DENYING`, section 4.1). With a period of 100 ms, the extra latency is at most 100
ms per step, well within the 500 ms of NFR-6.

## 3. Drone

### 3.1 Flight FSM

```mermaid
stateDiagram-v2
  [*] --> AT_REST
 
  state "AT_REST" as AT_REST
  AT_REST : charging = ON
  AT_REST : dynamics = idle
  state "TAKEOFF_REQ" as TAKEOFF_REQ
  TAKEOFF_REQ : on entry send TAKEOFF_REQ
  TAKEOFF_REQ : dynamics = idle
  state "LEAVING" as LEAVING
  LEAVING : dynamics = climb
  state "IN_MISSION" as IN_MISSION
  IN_MISSION : dynamics = follow segments
  state "RETURNING" as RETURNING
  RETURNING : dynamics = fly to hangar
  state "LANDING_REQ" as LANDING_REQ
  LANDING_REQ : on entry send LANDING_REQ
  LANDING_REQ : dynamics = hover
  state "AUTH_RESPONSE" as AUTH_RESPONSE
  AUTH_RESPONSE : on entry send AUTH_RESPONSE
  AUTH_RESPONSE : dynamics = hover
  state "HOLDING" as HOLDING
  HOLDING : dynamics = hover
  state "LANDING" as LANDING
  LANDING : dynamics = descend
  state "EMERGENCY" as EMERGENCY
  EMERGENCY : dynamics = emergency descent
  EMERGENCY : alarm = ON
 
  AT_REST --> TAKEOFF_REQ : startMission
  TAKEOFF_REQ --> LEAVING : granted
  TAKEOFF_REQ --> AT_REST : denied or timeout
  LEAVING --> IN_MISSION : altitude >= cruise altitude
  IN_MISSION --> RETURNING : missionDone or battery < return level or recall
  RETURNING --> LANDING_REQ : distance from hangar < approach distance
  LANDING_REQ --> AUTH_RESPONSE : challenge received
  LANDING_REQ --> HOLDING : denied or timeout
  AUTH_RESPONSE --> LANDING : granted
  AUTH_RESPONSE --> HOLDING : denied or timeout
  HOLDING --> LANDING_REQ : retry timer expired
  LANDING --> AT_REST : ground distance = 0
  IN_MISSION --> EMERGENCY : battery critical or fault
  RETURNING --> EMERGENCY : battery critical or fault
  HOLDING --> EMERGENCY : battery critical or fault
  EMERGENCY --> AT_REST : reset
```

Every state publishes its name on `dib/drone/state` when entered. The transitions to `EMERGENCY` drawn from `IN_MISSION`, `RETURNING` and `HOLDING` apply to **every flying state** (`LEAVING`, `LANDING_REQ`, `AUTH_RESPONSE`, `LANDING` included). They are omitted from the diagram for readability.

| State | Meaning |
| --- | --- |
| `AT_REST` | Inside the hangar, recharging. Initial state. |
| `TAKEOFF_REQ` | Take-off requested, waiting for the hangar's decision. |
| `LEAVING` | Leaving the hangar and climbing to cruise altitude. |
| `IN_MISSION` | Flying the mission segments. Reaching this state tells the hangar that the drone is out. |
| `RETURNING` | Flying back towards the hangar. |
| `LANDING_REQ` | Landing requested, hovering, waiting for the authentication challenge. |
| `AUTH_RESPONSE` | Challenge answered, waiting for the decision. |
| `HOLDING` | Landing denied or not answered: hovering until the retry timer expires. |
| `LANDING` | Descending into the hangar. Reaching `AT_REST` tells the hangar that the drone has landed. |
| `EMERGENCY` | Critical anomaly: emergency descent in the safe area (EC-6). Left only by an operator reset after recovery. |

| Variable | Use |
| --- | --- |
| `reqId` | Identifier of the current request, used to match the answer. |
| `timer` | Counts reactions, for timeouts and for the retry delay. |
| `segment`, `progress` | Current mission segment and travelled fraction. |
| `battery`, `altitude`, `groundDistance`, `distanceFromHangar`, `tilt` | Read from the continuous models. |
| `fault` | Set by the anomaly input (section 3.2). |

The guard `startMission` is true only if the command passed the dispatcher, which also checks that the battery is above the minimum take-off level. Otherwise it answers `BATTERY_LOW`.

### 3.2 Anomaly input FSM

A button, or the tilt switch if available, injects a fault to demonstrate the emergency behaviour. It uses the debouncing
FSM of section 4.4, and its event sets the `fault` variable read by the flight FSM.

## 4. Hangar

The hangar runs three FSMs: the **controller**, which takes decisions; the **door**, which executes them; the **environment monitor**,
which conditions the sensor inputs. A fourth component, the **status indicator**, maps states to LEDs and display.

### 4.1 Controller FSM

```mermaid
stateDiagram-v2
[*] --> IDLE

state "IDLE" as IDLE
IDLE : doorCmd = CLOSE
state "TAKEOFF_PREP" as TAKEOFF_PREP
TAKEOFF_PREP : doorCmd = OPEN
state "DEPARTURE" as DEPARTURE
DEPARTURE : doorCmd = OPEN
DEPARTURE : on entry send GRANTED
state "DRONE_OUT" as DRONE_OUT
DRONE_OUT : doorCmd = CLOSE
state "AUTHENTICATING" as AUTHENTICATING
AUTHENTICATING : doorCmd = CLOSE
AUTHENTICATING : on entry send CHALLENGE with nonce
state "LANDING_PREP" as LANDING_PREP
LANDING_PREP : doorCmd = OPEN
state "LANDING_WAIT" as LANDING_WAIT
LANDING_WAIT : doorCmd = OPEN
LANDING_WAIT : on entry send GRANTED
state "DENYING" as DENYING
DENYING : doorCmd = CLOSE
DENYING : on entry send DENIED with reason
state "ALARM" as ALARM
ALARM : doorCmd = CLOSE
ALARM : alarm = ON

IDLE --> TAKEOFF_PREP : TAKEOFF_REQ and envOk
IDLE --> DENYING : TAKEOFF_REQ and not envOk
TAKEOFF_PREP --> DEPARTURE : doorState = OPEN
DEPARTURE --> DRONE_OUT : droneState = IN_MISSION
DRONE_OUT --> AUTHENTICATING : LANDING_REQ
AUTHENTICATING --> LANDING_PREP : hmac ok and envOk
AUTHENTICATING --> DENYING : hmac ok and not envOk
AUTHENTICATING --> DENYING : hmac wrong or timeout
LANDING_PREP --> LANDING_WAIT : doorState = OPEN
LANDING_WAIT --> IDLE : droneState = AT_REST
DENYING --> IDLE : droneInside
DENYING --> DRONE_OUT : not droneInside
IDLE --> ALARM : emergencyClose
DRONE_OUT --> ALARM : emergencyClose
DEPARTURE --> ALARM : timeout or droneOffline
LANDING_WAIT --> ALARM : timeout or droneOffline
ALARM --> IDLE : reset and causeCleared and droneInside
ALARM --> DRONE_OUT : reset and causeCleared and not droneInside
```

Every state publishes its name on `dib/hangar/state` when entered.

| State | Meaning |
| --- | --- |
| `IDLE` | Drone inside, door closed. Initial state (EC-14). |
| `TAKEOFF_PREP` | Take-off accepted, door opening. |
| `DEPARTURE` | Door fully open, take-off granted, waiting for the drone to be out. |
| `DRONE_OUT` | Drone in mission, door closed. |
| `AUTHENTICATING` | Landing requested, challenge sent, waiting for the answer. |
| `LANDING_PREP` | Landing accepted, door opening. |
| `LANDING_WAIT` | Door fully open, landing granted, waiting for the drone to land. |
| `DENYING` | Transient: sends the denial with its reason, then returns at the next reaction. |
| `ALARM` | Door closed, every request denied until an operator reset. |

| Variable | Use |
| --- | --- |
| `droneInside` | Whether the drone is in the hangar. Set when entering `IDLE`, cleared when entering `DRONE_OUT`. |
| `reqId`, `nonce` | Current request and authentication challenge. |
| `reason` | Reason of the denial or of the alarm. |
| `timer` | Timeouts in `AUTHENTICATING`, `DEPARTURE` and `LANDING_WAIT`. |
| `emergencyPending` | Emergency closure requested while the drone is passing through the door (EC-15). |

Inputs: `envOk` and the reason come from the environment monitor (4.3). `doorState` comes from the door FSM (4.2).
`droneState` and `droneOffline` come from the drone's state topic and Last Will. Requests, the challenge answer and operator
commands come from the dispatcher.

### 4.2 Door FSM

```mermaid
stateDiagram-v2
  [*] --> CLOSED
 
  state "CLOSED" as CLOSED
  CLOSED : servo angle = 0°
  state "OPENING" as OPENING
  OPENING : servo angle = angle
  state "OPEN" as OPEN
  OPEN : servo angle = 90°
  state "CLOSING" as CLOSING
  CLOSING : servo angle = angle
 
  CLOSED --> OPENING : doorCmd = OPEN
  OPENING --> OPEN : angle >= 90°
  OPENING --> CLOSING : doorCmd = CLOSE
  OPEN --> CLOSING : doorCmd = CLOSE
  CLOSING --> CLOSED : angle <= 0°
  CLOSING --> OPENING : doorCmd = OPEN
```

In `OPENING` and `CLOSING` the variable `angle` increases or decreases by a fixed step at every reaction (e.g. 3° every 50 ms, a full movement in 1.5 s). The door state and its position in % are published at every change.

The door FSM is a pure executor: it never decides whether opening is safe. That decision belongs to the controller.


### 4.3 Environment monitor FSM

Raw readings are noisy, and a value near a threshold would make the classification flip at every sample. The monitor applies **input conditioning**: a change of classification requires several consecutive readings that agree.

```mermaid
stateDiagram-v2
  [*] --> SAFE
 
  state "SAFE" as SAFE
  SAFE : envOk = true
  state "UNSAFE" as UNSAFE
  UNSAFE : envOk = false, reason
  state "SENSOR_FAULT" as SENSOR_FAULT
  SENSOR_FAULT : envOk = false, reason = SENSOR_FAULT
 
  SAFE --> UNSAFE : unsafeCount >= 2
  UNSAFE --> SAFE : safeCount >= 3
  SAFE --> SENSOR_FAULT : read error
  UNSAFE --> SENSOR_FAULT : read error
  SENSOR_FAULT --> UNSAFE : valid reading
```

`unsafeCount` and `safeCount` count consecutive readings that violate, or respect, the thresholds of the safety policy, and are reset when a reading disagrees. Leaving the unsafe classification requires more readings than entering it, so the system is cautious. After a sensor fault the monitor goes back through `UNSAFE`, so a single valid reading is not enough to declare the environment safe.

### 4.4 Status indicator

The status indicator is not an independent decision maker: its output is a function of the controller state. It is a Moore machine whose only internal state is the blinking phase, which toggles every 250 ms.

| Controller state | Green | Yellow | Red | Display |
| --- | --- | --- | --- | --- |
| `IDLE` | on | off | off | state, temperature, humidity |
| `TAKEOFF_PREP`, `LANDING_PREP` | off | blinking | off | "DOOR OPENING" |
| `DEPARTURE`, `LANDING_WAIT` | off | on | off | "TAKE-OFF" / "LANDING" |
| `DRONE_OUT` | blinking | off | off | "DRONE OUT", environment |
| `AUTHENTICATING` | off | blinking | off | "AUTHENTICATING" |
| `DENYING` | off | off | on | denial reason |
| `ALARM` | off | off | blinking | alarm reason |

## 5. Interaction between drone and hangar

The two flight-critical interactions are request/response exchanges, matched by `reqId` and bounded by timeouts on both sides (NFR-3).

### 5.1 Take-off

```mermaid
sequenceDiagram
  participant D as Drone
  participant H as Hangar
  participant DR as Door
 
  Note over D: AT_REST
  D->>H: TAKEOFF_REQ (reqId)
  Note over D: TAKEOFF_REQ
  Note over H: IDLE, envOk
  H->>DR: doorCmd = OPEN
  Note over H: TAKEOFF_PREP
  DR-->>H: doorState = OPEN
  H->>D: GRANTED (reqId)
  Note over H: DEPARTURE
  Note over D: LEAVING
  D-->>H: state = IN_MISSION
  H->>DR: doorCmd = CLOSE
  Note over H: DRONE_OUT
```

### 5.2 Landing with authentication

```mermaid
sequenceDiagram
  participant D as Drone
  participant H as Hangar
  participant DR as Door
 
  Note over D: RETURNING
  D->>H: LANDING_REQ (reqId)
  Note over D: LANDING_REQ
  H->>D: CHALLENGE (reqId, nonce)
  Note over H: AUTHENTICATING
  D->>H: AUTH_RESPONSE (reqId, HMAC(key, nonce, droneId, reqId))
  Note over D: AUTH_RESPONSE
  Note over H: hmac ok, envOk
  H->>DR: doorCmd = OPEN
  Note over H: LANDING_PREP
  DR-->>H: doorState = OPEN
  H->>D: GRANTED (reqId)
  Note over H: LANDING_WAIT
  Note over D: LANDING
  D-->>H: state = AT_REST
  H->>DR: doorCmd = CLOSE
  Note over H: IDLE
```

### 5.3 Landing denied

```mermaid
sequenceDiagram
  participant D as Drone
  participant H as Hangar
 
  D->>H: LANDING_REQ (reqId = 7)
  H->>D: CHALLENGE (7, nonce)
  D->>H: AUTH_RESPONSE (7, hmac)
  Note over H: hmac ok, not envOk
  H->>D: DENIED (7, WATER_DETECTED)
  Note over H: DENYING → DRONE_OUT
  Note over D: HOLDING, retry timer
  D->>H: LANDING_REQ (reqId = 8)
  Note over D,H: same exchange, with a new nonce
```
## 6. Tasks and periods

Each FSM is executed by one task of a **cooperative scheduler**. The continuous models are tasks too, since they advance
with a fixed time step.

| Device | Task | Executes | Period |
| --- | --- | --- | --- |
| Hangar | Comm | MQTT client, dispatcher, message queue | 50 ms |
| Hangar | Controller | Controller FSM (4.1) | 100 ms |
| Hangar | Door | Door FSM (4.2) | 50 ms |
| Hangar | EnvMonitor | Environment monitor FSM (4.3) | 2 s |
| Hangar | Status | Status indicator (4.5) | 250 ms |
| Drone | Comm | MQTT client, dispatcher, message queue | 50 ms |
| Drone | Flight | Flight FSM (3.1) | 100 ms |
| Drone | FlightModel | Kinematics, `dt` = 100 ms | 100 ms |
| Drone | Battery | Battery model | 1 s |
| Drone | Anomaly | Button FSM for the anomaly input (3.2) | 50 ms |
| Drone | Telemetry | Publishes telemetry | 500 ms |

The choice of the periods follows the rule from the theory: *the largest period that still guarantees that no event is lost and that latencies stay acceptable.*

- **100 ms** for the decision-making FSMs gives a worst-case reaction latency well below the 500 ms of NFR-6.
- **50 ms** for the door gives a smooth movement. For the buttons it filters bounces while staying below the MEST of a press.
- **2 s** for the environment: the weather changes slowly, and the DHT11 sensor must not be read more than once per second.
- **The scheduler runs at 50 ms**, the greatest common divisor of all periods.
  
**Messages and the MEST.** For physical inputs, choosing a period shorter than the MEST guarantees that no event is lost.
Network messages give no such guarantee: two messages can arrive a few milliseconds apart. For this reason the MQTT callback
never touches the FSMs: it pushes each message into a queue, and the dispatcher delivers the queued events to the machines
at their next reaction. The queue is the boundary between the asynchronous world of the network and the synchronous world of the FSMs.