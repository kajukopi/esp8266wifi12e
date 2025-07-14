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
    body { margin: 0; font-family: Arial, sans-serif; }
    .sidebar {
      height: 100%;
      width: 0;
      position: fixed;
      z-index: 1000;
      top: 0; left: 0;
      background-color: #0d6efd;
      overflow-x: hidden;
      transition: 0.3s;
      padding-top: 2rem;
      color: white;
    }
    .sidebar a {
      display: block;
      color: white;
      padding: 1rem 1.5rem;
      text-decoration: none;
    }
    .sidebar a:hover { background-color: #0b5ed7; }
    .sidebar .closebtn {
      position: absolute;
      top: 0.5rem;
      right: 1rem;
      font-size: 30px;
    }
    .main-content { margin-left: 0; padding: 0.5rem; transition: margin-left 0.3s; }
    .card { margin-bottom: 1rem; }
    .navbar { z-index: 1100; }
    .log-block {
      background: #f8f9fa;
      padding: 1rem;
      border-radius: 10px;
      font-size: 0.85rem;
      max-height: 250px;
      overflow-y: auto;
      font-family: monospace;
    }
  </style>
</head>
<body>
  <!-- Sidebar -->
  <div id="mySidebar" class="sidebar mt-5">
    <a href="javascript:void(0)" class="closebtn" onclick="closeSidebar()">&times;</a>
    <h5 class="text-center mt-3"><i class="fas fa-microchip"></i> ESP8266</h5>
    <a href="#" onclick="showPage('home')"><i class="fas fa-home"></i> Home</a>
    <a href="#" onclick="showPage('update')"><i class="fas fa-upload"></i> OTA Update</a>
  </div>

  <!-- Navbar -->
  <nav class="navbar navbar-expand-lg navbar-dark bg-primary">
    <div class="container-fluid">
      <button class="btn btn-light btn-sm" onclick="openSidebar()">
        <i class="fas fa-bars text-primary"></i>
      </button>
      <span class="navbar-brand ms-3">ESP8266 Control Panel</span>
    </div>
  </nav>

  <!-- Main Content -->
  <div class="main-content">
    <div class="container">
      <!-- Home Page -->
      <div id="home-page">
        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-satellite-dish"></i> Device Info</h5>
            <p><i class="fas fa-wifi"></i> IP Address: <span id="ip">-</span></p>
            <p><i class="fas fa-signal"></i> Signal Strength: <span id="signal">-</span></p>
          </div>
        </div>

        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-sliders-h"></i> Servo Control</h5>
            <p>Angle: <span id="angleValue">0%</span></p>
            <input type="range" class="form-range" id="slider" min="0" max="100" value="0">
          </div>
        </div>

        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-lightbulb"></i> LED Control</h5>
            <div class="form-check form-switch d-flex justify-content-center">
              <input class="form-check-input" type="checkbox" id="ledSwitch">
              <label class="form-check-label ms-2" for="ledSwitch">LED Status</label>
            </div>
          </div>
        </div>

        <!-- ✅ Relay Control -->
        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-plug"></i> Relay Control</h5>
            <div class="form-check form-switch d-flex justify-content-center">
              <input class="form-check-input" type="checkbox" id="relaySwitch">
              <label class="form-check-label ms-2" for="relaySwitch">Relay Status</label>
            </div>
          </div>
        </div>

        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-terminal"></i> Firebase Log</h5>
            <div id="firebaseLog" class="log-block"></div>
          </div>
        </div>

        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-terminal"></i> ESP Log</h5>
            <div id="espLog" class="log-block"></div>
          </div>
        </div>
      </div>

      <!-- Update Page -->
      <div id="update-page" style="display:none">
        <div class="card shadow-2-strong text-center">
          <div class="card-body">
            <h5 class="card-title"><i class="fas fa-upload"></i> OTA Firmware Update</h5>
            <form method="POST" action="/update" enctype="multipart/form-data" class="mt-4">
              <input type="file" name="firmware" class="form-control mb-3" required>
              <input type="submit" value="Upload" class="btn btn-success">
            </form>
          </div>
        </div>
      </div>
    </div>
  </div>

<!-- Firebase & Control Script -->
<script src="https://www.gstatic.com/firebasejs/8.10.1/firebase-app.js"></script>
<script src="https://www.gstatic.com/firebasejs/8.10.1/firebase-database.js"></script>
<script>
  const firebaseConfig = {
    apiKey: "AIzaSyBczsujBWZbP2eq5C1YR1JF3xPixWVYnxY",
    authDomain: "payunghitam.firebaseapp.com",
    databaseURL: "https://payunghitam-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "payunghitam",
    storageBucket: "payunghitam.firebasestorage.app",
    messagingSenderId: "600763621790",
    appId: "1:600763621790:web:7a21e49198a53e4142b68f"
  };

  firebase.initializeApp(firebaseConfig);
  const db = firebase.database();

  const slider = document.getElementById('slider');
  const angleValue = document.getElementById('angleValue');
  const ledSwitch = document.getElementById('ledSwitch');
  const relaySwitch = document.getElementById('relaySwitch'); // ✅ new
  const logBox = document.getElementById("firebaseLog");

  // Firebase listener → update UI + kirim ke ESP
  db.ref("/device").on("value", snapshot => {
    const data = snapshot.val();
    if (!data) return;

    const ledState = data.led;
    const servoValue = data.servo;
    const relayState = data.relay;

    // Update UI
    if (typeof ledState === 'boolean') ledSwitch.checked = ledState;
    if (typeof servoValue === 'number') {
      slider.value = servoValue;
      angleValue.textContent = `${servoValue}%`;
    }
    if (typeof relayState === 'boolean') relaySwitch.checked = relayState; // ✅

    // Log perubahan
    const logLine = `[FIREBASE] LED: ${ledState ? 'ON' : 'OFF'}, Servo: ${servoValue}%, Relay: ${relayState ? 'ON' : 'OFF'}`;
    logBox.textContent += logLine + "\n";
    logBox.scrollTop = logBox.scrollHeight;

    // Kirim ke ESP
    fetch("/toggleLED?state=" + (ledState ? "on" : "off"));
    fetch("/setServo?percent=" + servoValue);
    fetch("/toggleRelay?state=" + (relayState ? "on" : "off")); // ✅
  });

  // User control → simpan ke Firebase
  if (ledSwitch) {
    ledSwitch.onchange = () => {
      const state = ledSwitch.checked;
      db.ref("/device/led").set(state);
    };
  }

  if (relaySwitch) {  // ✅
    relaySwitch.onchange = () => {
      const state = relaySwitch.checked;
      db.ref("/device/relay").set(state);
    };
  }

  if (slider) {
    slider.oninput = () => {
      angleValue.textContent = slider.value + "%";
      db.ref("/device/servo").set(parseInt(slider.value));
    };
  }

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
        const logEl = document.getElementById("espLog");
        logEl.textContent = data;
        logEl.scrollTop = logEl.scrollHeight;
      });
  }

  // Loop
  setInterval(updateStatus, 3000);
  setInterval(updateESPLog, 3000);
  updateStatus();
  updateESPLog();
  showPage('home');

  function openSidebar() {
    document.getElementById("mySidebar").style.width = "220px";
  }
  function closeSidebar() {
    document.getElementById("mySidebar").style.width = "0";
  }
  function showPage(page) {
    document.getElementById("home-page").style.display = (page === "home") ? "block" : "none";
    document.getElementById("update-page").style.display = (page === "update") ? "block" : "none";
    closeSidebar();
  }
</script>
</body>
</html>
)rawliteral";

#endif