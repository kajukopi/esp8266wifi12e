const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>
    body {
      margin: 0;
      font-family: Arial, sans-serif;
    }
    .sidebar {
      height: 100vh;
      width: 220px;
      position: fixed;
      top: 0;
      left: 0;
      background-color: #0d6efd;
      padding-top: 2rem;
      color: white;
    }
    .sidebar a {
      display: block;
      color: white;
      padding: 1rem 1.5rem;
      text-decoration: none;
    }
    .sidebar a:hover {
      background-color: #0b5ed7;
    }
    .main-content {
      margin-left: 220px;
      padding: 2rem;
    }
    .card {
      margin-bottom: 2rem;
    }
  </style>
</head>
<body>

  <!-- Sidebar -->
  <div class="sidebar">
    <h5 class="text-center"><i class="fas fa-microchip"></i> ESP8266</h5>
    <a href="/"><i class="fas fa-home"></i> Home</a>
    <a href="/update"><i class="fas fa-upload"></i> OTA Update</a>
  </div>

  <!-- Main Content -->
  <div class="main-content">
    <div class="container">
      <!-- Device Info -->
      <div class="card shadow-2-strong">
        <div class="card-body text-center">
          <h5 class="card-title"><i class="fas fa-satellite-dish"></i> Device Info</h5>
          <p><i class="fas fa-wifi"></i> IP Address: <span id="ip">-</span></p>
          <p><i class="fas fa-signal"></i> Signal Strength: <span id="signal">-</span></p>
        </div>
      </div>

      <!-- Servo Control -->
      <div class="card shadow-2-strong">
        <div class="card-body text-center">
          <h5 class="card-title"><i class="fas fa-sliders-h"></i> Servo Control</h5>
          <p>Angle: <span id="angleValue">0%</span></p>
          <input type="range" class="form-range" id="slider" min="0" max="100" value="0">
        </div>
      </div>

      <!-- LED Control -->
      <div class="card shadow-2-strong">
        <div class="card-body text-center">
          <h5 class="card-title"><i class="fas fa-lightbulb"></i> LED Control</h5>
          <div class="form-check form-switch d-flex justify-content-center">
            <input class="form-check-input" type="checkbox" id="ledSwitch">
            <label class="form-check-label ms-2" for="ledSwitch">LED Status</label>
          </div>
        </div>
      </div>
    </div>
  </div>

  <!-- Script -->
  <script>
    const slider = document.getElementById('slider');
    const angleValue = document.getElementById('angleValue');
    const ledSwitch = document.getElementById('ledSwitch');

    slider.oninput = () => {
      angleValue.textContent = slider.value + "%";
      fetch("/setServo?percent=" + slider.value);
    };

    ledSwitch.onchange = () => {
      fetch("/toggleLED?state=" + (ledSwitch.checked ? "on" : "off"));
    };

    function updateStatus() {
      fetch("/status")
        .then(res => res.json())
        .then(data => {
          document.getElementById('ip').textContent = data.ip;
          document.getElementById('signal').textContent = data.signal;
        });
    }

    setInterval(updateStatus, 2000);
    updateStatus();
  </script>

</body>
</html>
)rawliteral";