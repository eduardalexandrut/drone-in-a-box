# Analysis

## 1. Project goal

The goal of the project is to build a working prototype of a **Drone-in-a-Box (DIB)** system: an autonomous drone that lives in a ground station (the *hangar*), leaves it to perform automated industrial inspections (e.g. thermographic monitoring of a photovoltaic plant) and comes back to recharge, without a human pilot.

The prototype focuses on the **coordination and safety logic** between the drone and the hangar, rather than on real flight:

- the **hangar** is a physical device that controls its door, monitors the local environment and decides autonomously whether a take-off or a landing is safe;
- the **drone** is a physical board that *simulates* a flight and streams realistic telemetry;
- a **central backend** supervises the whole system, shows its live state on a dashboard and lets an operator issue commands.
  The three subsystems are independent and communicate only through messages, following a publish/subscribe architecture.

### 1.1 What is real and what is simulated

| Aspect | Real | Simulated |
| --- | --- | --- |
| Hangar door | servo motor | – |
| Environmental data | temperature, humidity and water/steam sensors | – (can be injected for tests) |
| Status indicators | LEDs and display | – |
| Drone flight | – | speed, acceleration, altitude, ground distance, distance from the hangar, mission progress |
| Drone battery | – | charge and discharge model |
| Drone attitude (tilt) | tilt sensor, if available in the kit | roll/pitch angles otherwise |
| Communication | Wi-Fi + MQTT between real devices | – |

## 2. Requirements

Requirements are numbered so that they can be traced to the design and to the tests.

### 2.1 Functional requirements

**Hangar**

| ID | Requirement |
| --- | --- |
| FR-H1 | The hangar shall open and close its door on its own decision, moving it gradually. |
| FR-H2 | The hangar shall periodically sample temperature, humidity and water/steam presence. |
| FR-H3 | The hangar shall classify the environment as *safe* or *unsafe* according to configurable thresholds, and report the reason when unsafe. |
| FR-H4 | The hangar shall evaluate every take-off request and grant it only if the environment is safe and the drone is inside. |
| FR-H5 | The hangar shall authenticate the drone before evaluating a landing request. |
| FR-H6 | The hangar shall evaluate every landing request and grant it only if the drone is authenticated, the environment is safe and the drone is stable (tilt and speed within limits). |
| FR-H7 | The hangar shall notify a grant only when the door is fully open, and close the door once the drone has left or landed. |
| FR-H8 | Every denial shall carry a machine-readable reason. |
| FR-H9 | The hangar shall show its current state through LEDs and display. |
| FR-H10 | The hangar shall publish its state, door position and environmental data. |

**Drone**

| ID | Requirement |
| --- | --- |
| FR-D1 | While at rest in the hangar, the drone shall recharge its (simulated) battery. |
| FR-D2 | The drone shall request permission before taking off and before landing, and act only after a grant. |
| FR-D3 | The drone shall simulate a mission as a sequence of segments, and report its progress. |
| FR-D4 | The drone shall simulate and publish telemetry: battery level, speed, acceleration, altitude, ground distance, distance from the hangar, tilt and error state. |
| FR-D5 | The drone shall return to the hangar when the mission is complete, when the battery falls below a return threshold, or when recalled by the operator. |
| FR-D6 | If a landing is denied, the drone shall hold its position and retry periodically. |
| FR-D7 | The drone shall enter an emergency state on critical anomalies (excessive tilt, critical battery, internal fault). |
| FR-D8 | The drone shall answer the hangar's authentication challenge. |

**Backend and dashboard**

| ID | Requirement |
| --- | --- |
| FR-B1 | The backend shall collect the state and data published by the hangar and the drone. |
| FR-B2 | The dashboard shall show, in real time, the hangar state, door state and environmental data (with recent history). |
| FR-B3 | The dashboard shall show, in real time, the drone state, battery, kinematics, distance from the hangar, tilt, mission progress and errors. |
| FR-B4 | The dashboard shall show whether each device is online or offline. |
| FR-B5 | The dashboard shall keep a log of state transitions, requests and decisions. |
| FR-B6 | The operator shall be able to start a mission, recall the drone, trigger an emergency door closure and reset an alarm. |
| FR-B7 | Operator commands are requests: the devices may refuse them, and every refusal shall be reported with a reason. |

### 2.2 Non-functional requirements

| ID | Category | Requirement |
| --- | --- | --- |
| NFR-1 | Safety | Safety-critical decisions (opening the door, granting take-off or landing) shall be taken locally by the hangar and shall not depend on the backend being available. |
| NFR-2 | Safety | When communication with the drone is lost during an operation, the hangar shall move to a safe state. |
| NFR-3 | Reliability | Every request shall be answered or time out; no device shall wait forever. |
| NFR-4 | Reliability | Devices shall reconnect automatically after a Wi-Fi or broker outage and keep working autonomously while disconnected. |
| NFR-5 | Reliability | The disconnection of a device shall be detected by the others and by the backend. |
| NFR-6 | Responsiveness | The hangar shall react to a request within 500 ms (excluding the door movement). |
| NFR-7 | Responsiveness | The dashboard shall show telemetry with a delay of at most 2 s. |
| NFR-8 | Security | Every client shall authenticate to the message broker; anonymous access shall be disabled. |
| NFR-9 | Security | Each client shall be allowed to publish and subscribe only to the topics required by its role. |
| NFR-10 | Security | The drone authentication shall resist replay of previously captured messages. |
| NFR-11 | Maintainability | The control logic shall be independent of the hardware, so that it can be tested on a PC. |
| NFR-12 | Testability | Each device shall be replaceable by a software simulator implementing the same protocol. |
| NFR-13 | Extensibility | The message format and topic structure shall allow adding more drones or hangars in the future. |

### 2.3 Constraints

- Hardware: 2 × Raspberry Pi Pico 2 W, 9 g servo motor, environmental sensors, LEDs and display (Keyestudio-style kit).
- Communication: MQTT over Wi-Fi (TCP/IP).
- Firmware language: C++.
- Backend: runs on a PC (broker, supervision logic, dashboard).

## 3. Domain model
 TODO

## 4. Use cases

### 4.1 Actors

- **Operator**: monitors the system and issues commands from the dashboard.
- **Drone**: autonomous actor that requests take-off and landing.
- **Hangar**: autonomous actor that decides on requests and controls the door.
- **Environment**: external source of weather conditions (time, temperature, humidity, rain).
### 4.2 Use case overview

| ID | Use case | Primary actor |
| --- | --- | --- |
| UC-1 | Monitor the system | Operator |
| UC-2 | Start a mission | Operator |
| UC-3 | Take off | Drone |
| UC-4 | Execute a mission | Drone |
| UC-5 | Return and land | Drone |
| UC-6 | Recall the drone | Operator |
| UC-7 | Emergency door closure | Operator |
| UC-8 | Reset an alarm | Operator |

### 4.3 Main scenarios

**UC-1 Monitor the system**

1. The operator opens the dashboard.
2. The dashboard shows the latest known state of hangar and drone, even if they published it before the dashboard was opened.
3. The dashboard updates state, environment and telemetry as new data arrives, and logs every transition and decision.
4. If a device goes offline, the dashboard marks it as offline.
   **UC-2 Start a mission**

*Precondition:* the drone is at rest inside the hangar.

1. The operator presses *Start mission*.
2. The supervision unit forwards the command to the drone.
3. The drone checks that its battery is above the minimum take-off level.
4. The drone starts UC-3.
   *Alternative 3a:* the battery is too low → the drone refuses the command with reason `BATTERY_LOW`; the dashboard shows the refusal.

**UC-3 Take off**

*Precondition:* the drone is at rest inside the hangar, the door is closed.

1. The drone sends a take-off request.
2. The hangar evaluates the environment: it is safe.
3. The hangar opens the door.
4. When the door is fully open, the hangar grants the take-off.
5. The drone leaves the hangar and climbs to cruise altitude.
6. The drone notifies that it has left; the hangar closes the door.
7. The drone is in mission (UC-4).
   *Alternative 2a:* the environment is unsafe → the hangar denies the request with the reason (e.g. `HUMIDITY_HIGH`, `WATER_DETECTED`); the door stays closed; the drone stays at rest.

*Alternative 1a:* the hangar does not answer within the timeout → the drone gives up and stays at rest, reporting `NO_RESPONSE`.

**UC-4 Execute a mission**

1. The drone flies the mission segments, accelerating and decelerating on each one.
2. The drone publishes telemetry and mission progress periodically.
3. When the last segment is completed, the drone starts UC-5.
   *Alternative 1a:* the battery falls below the return threshold → the drone interrupts the mission and starts UC-5.
   *Alternative 1b:* a critical anomaly occurs (excessive tilt, fault) → the drone enters the emergency state and reports it.

**UC-5 Return and land**

1. The drone flies back towards the hangar.
2. When it is close enough, the drone sends a landing request.
3. The hangar sends an authentication challenge.
4. The drone answers the challenge; the hangar verifies it.
5. The hangar checks environment and drone stability: both are fine.
6. The hangar opens the door; when it is fully open, it grants the landing.
7. The drone descends and notifies that it has landed.
8. The hangar closes the door; the drone is at rest and starts recharging.
   *Alternative 4a:* authentication fails → the hangar denies the landing with reason `AUTH_FAILED`; the door stays closed.
   *Alternative 5a:* environment unsafe or drone unstable → the hangar denies the landing with the reason; the drone holds its position and retries periodically (back to step 2).

**UC-6 Recall the drone**

*Precondition:* the drone is in mission.

1. The operator presses *Recall*.
2. The drone interrupts the mission and starts UC-5.
   *Alternative 2a:* the drone is not in mission → the command is refused with reason `INVALID_STATE`.

**UC-7 Emergency door closure**

1. The operator presses *Emergency close*.
2. The hangar closes the door and enters the alarm state; it denies new requests until reset.
   *Alternative 2a:* the drone is passing through the door (taking off or landing) → see edge case EC-15.

**UC-8 Reset an alarm**

1. The operator presses *Reset alarm*.
2. If the cause of the alarm is no longer present, the hangar leaves the alarm state; otherwise it refuses with the cause as reason.

## 5. Edge cases

Edge cases describe situations outside the main scenarios and the expected behaviour of the system. Each one becomes at 
least one test scenario.

| ID    | Situation | Expected behaviour |
|-------| --- | --- |
| EC-1  | The environment becomes unsafe while the door is open for a take-off, before the drone has left. | The take-off already granted is completed (aborting halfway is more dangerous); the hangar closes the door right after and denies new requests. |
| EC-2  | After a take-off grant, the drone never reports that it has left. | After a timeout the hangar closes the door only if the drone is confirmed inside or offline, and raises an alarm. |
| EC-3  | The drone loses connection while landing (door open). | The hangar detects the disconnection, waits for a grace period, then closes the door and raises an alarm. |
| EC-4  | The hangar loses connection while the drone is in mission. | The drone completes its mission; landing requests time out, so the drone holds and retries until the hangar is back or the battery becomes critical (EC-6). |
| EC-5  | The backend is not available. | Drone and hangar keep coordinating directly; only monitoring and operator commands are lost. |
| EC-6  | A device that is not the registered drone, or uses a wrong key, requests a landing. | Authentication fails; the landing is denied with `AUTH_FAILED`; the door stays closed. |
| EC-7  | A previously captured authentication answer is sent again (replay). | The answer does not match the new challenge; authentication fails. |
| EC-8  | A message is duplicated, or an answer arrives for an old request. | Answers are matched by `reqId`; answers for unknown or expired requests are ignored. |
| EC-9  | An operator command is not valid in the current state (e.g. *Start mission* while in mission). | The command is refused with `INVALID_STATE`; the state does not change. |
| EC-10 | The broker restarts, or the dashboard connects after the devices. | The dashboard receives the last known state of each device as soon as it subscribes. |
| EC-11 | The hangar reboots while the drone is in mission. | At start-up the hangar assumes the door must be closed and the drone outside unless the drone's last published state says otherwise. |
| EC-12 | An emergency closure is requested while the drone is passing through the door. | The hangar completes the passage before closing (it does not close the door onto the drone), then enters the alarm state. |
