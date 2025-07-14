#ifndef WEBPAGE_H
#define WEBPAGE_H

const char WEB_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Panel</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>
    body { margin: 0; font-family: Arial, sans-serif; background-color: #f2f2f2; }
    .card { margin: 1rem 0; }
    .log-block {
      background: #fff;
      padding: 1rem;
      border-radius: 10px;
      font-size: 0.85rem;
      max-height: 250px;
      overflow-y: auto;
      font-family: monospace;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
  </style>
</head>
<body class="bg-light">

  <div class="container mt-4">
    <h4 class="text-center mb-4"><i class="fas fa-microchip"></i> ESP8266 Control Panel</h4>

    <div class="card">
      <div class="card-body text-center">
        <h5><i class="fas fa-satellite-dish"></i> Device Info</h5>
        <p><i class="fas fa-wifi"></i> IP Address: <span id="ip">-</span></p>
        <p><i class="fas fa-signal"></i> Signal Strength: <span id="signal">-</span></p>
      </div>
    </div>

    <div class="card">
      <div class="card-body text-center">
        <h5><i class="fas fa-sliders-h"></i> Servo Control</h5>
        <p>Angle: <span id="angleValue">0%</span></p>
        <input type="range" class="form-range" id="slider" min="0" max="100" value="0">
      </div>
    </div>

    <div class="card">
      <div class="card-body text-center">
        <h5><i class="fas fa-lightbulb"></i> LED Control</h5>
        <div class="form-check form-switch d-flex justify-content-center">
          <input class="form-check-input" type="checkbox" id="ledSwitch">
          <label class="form-check-label ms-2" for="ledSwitch">LED Status</label>
        </div>
      </div>
    </div>

    <div class="card">
      <div class="card-body text-center">
        <h5><i class="fas fa-plug"></i> Relay Control</h5>
        <div class="form-check form-switch d-flex justify-content-center">
          <input class="form-check-input" type="checkbox" id="relaySwitch">
          <label class="form-check-label ms-2" for="relaySwitch">Relay Status</label>
        </div>
      </div>
    </div>

    <div class="card">
      <div class="card-body">
        <h5><i class="fas fa-terminal"></i> ESP Log</h5>
        <div id="espLog" class="log-block"></div>
      </div>
    </div>
  </div>

<!-- Firebase -->
<script src="https://www.gstatic.com/firebasejs/8.10.1/firebase-app.js"></script>
<script src="https://www.gstatic.com/firebasejs/8.10.1/firebase-database.js"></script>
<script>
  const firebaseConfig = {
    apiKey: "AIzaSyBczsujBWZbP2eq5C1YR1JF3xPixWVYnxY",
    authDomain: "payunghitam.firebaseapp.com",
    databaseURL: "https://payunghitam-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "payunghitam",
    storageBucket: "payunghitam.appspot.com",
    messagingSenderId: "600763621790",
    appId: "1:600763621790:web:7a21e49198a53e4142b68f"
  };
  firebase.initializeApp(firebaseConfig);
  const db = firebase.database();
</script>

<!-- Control Logic -->
<script>
  const slider = document.getElementById('slider');
  const angleValue = document.getElementById('angleValue');
  const ledSwitch = document.getElementById('ledSwitch');
  const relaySwitch = document.getElementById('relaySwitch');
  const logEl = document.getElementById("espLog");

  // Update device info and logs
  function updateStatus() {
    fetch("/status")
      .then(res => res.json())
      .then(data => {
        document.getElementById('ip').textContent = data.ip;
        document.getElementById('signal').textContent = data.signal;
      });
  }

  function updateESPLog() {
    fetch("/log")
      .then(res => res.text())
      .then(data => {
        logEl.textContent = data;
        logEl.scrollTop = logEl.scrollHeight;
      });
  }

  // Servo Control
  if (slider) {
    slider.oninput = () => {
      const val = parseInt(slider.value);
      angleValue.textContent = val + "%";
      fetch("/setServo?percent=" + val)
        .then(() => db.ref("/device/servo").set(val));
    };
  }

  // LED Control
  if (ledSwitch) {
    ledSwitch.onchange = () => {
      const state = ledSwitch.checked ? "on" : "off";
      fetch("/toggleLED?state=" + state)
        .then(() => db.ref("/device/led").set(ledSwitch.checked));
    };
  }

  // Relay Control
  if (relaySwitch) {
    relaySwitch.onchange = () => {
      const state = relaySwitch.checked ? "on" : "off";
      fetch("/toggleRelay?state=" + state)
        .then(() => db.ref("/device/relay").set(relaySwitch.checked));
    };
  }

  setInterval(updateStatus, 3000);
  setInterval(updateESPLog, 3000);
  updateStatus();
  updateESPLog();
</script>

</body>
</html>
)rawliteral";

#endif