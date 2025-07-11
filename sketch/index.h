const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>body{padding:2rem}</style>
</head>
<body class="container text-center">
  <h3><i class="fas fa-microchip"></i> ESP8266 Control Panel</h3>
  <p><i class="fas fa-wifi"></i> IP: <span id="ip">-</span></p>
  <p><i class="fas fa-signal"></i> Signal: <span id="signal">-</span></p>

  <div class="mt-4">
    <label><i class="fas fa-sliders-h"></i> Servo Angle: <span id="angleValue">0%</span></label>
    <input type="range" class="form-range" id="slider" min="0" max="100" value="0">
  </div>

  <div class="form-check form-switch mt-4">
    <input class="form-check-input" type="checkbox" id="ledSwitch">
    <label class="form-check-label"><i class="fas fa-lightbulb"></i> LED</label>
  </div>

  <a href="/update" class="btn btn-primary mt-4"><i class="fas fa-upload"></i> OTA Update</a>

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