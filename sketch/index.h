const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>ESP8266 Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>
    body { padding: 2rem; }
    .servo-label { margin-top: 2rem; }
  </style>
</head>
<body class="container text-center">
  <h2><i class="fas fa-microchip"></i> ESP8266 Control Panel</h2>
  <p><i class="fas fa-wifi"></i> IP: <span id="ip">%IP%</span></p>
  <p><i class="fas fa-signal"></i> Signal: <span id="signal">-</span></p>

  <div class="servo-label">
    <label class="form-label"><i class="fas fa-sliders-h"></i> Servo: <span id="angleValue">0%</span></label>
    <input type="range" class="form-range" id="slider" min="0" max="100" value="0">
  </div>

  <div class="form-check form-switch mt-4">
    <input class="form-check-input" type="checkbox" id="ledSwitch">
    <label class="form-check-label" for="ledSwitch"><i class="fas fa-lightbulb"></i> LED</label>
  </div>

  <a href="/update" class="btn btn-primary mt-4"><i class="fas fa-upload"></i> OTA Update</a>

  <script>
    const slider = document.getElementById('slider');
    const ledSwitch = document.getElementById('ledSwitch');
    const angleValue = document.getElementById('angleValue');
    const signalSpan = document.getElementById('signal');
    const ipSpan = document.getElementById('ip');

    let socket = new WebSocket(`ws://${location.host}/ws`);

    socket.onmessage = function(event) {
      let data = JSON.parse(event.data);
      if (data.signal) signalSpan.textContent = data.signal;
      if (data.ip) ipSpan.textContent = data.ip;
    }

    slider.addEventListener('input', () => {
      angleValue.textContent = slider.value + '%';
      socket.send(JSON.stringify({ servo: slider.value }));
    });

    ledSwitch.addEventListener('change', () => {
      socket.send(JSON.stringify({ led: ledSwitch.checked ? "on" : "off" }));
    });
  </script>
</body>
</html>
)rawliteral";