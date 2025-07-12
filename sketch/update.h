const char UPDATE_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>OTA Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>
    body { padding-top: 4rem; }
    .card { margin-top: 2rem; }
  </style>
</head>
<body>
  <!-- Navbar -->
  <nav class="navbar navbar-expand-lg navbar-dark bg-primary fixed-top">
    <div class="container-fluid">
      <a class="navbar-brand" href="#"><i class="fas fa-microchip"></i> ESP8266</a>
      <button class="navbar-toggler" type="button" data-mdb-toggle="collapse" data-mdb-target="#navbarNav">
        <span class="navbar-toggler-icon"></span>
      </button>
      <div class="collapse navbar-collapse" id="navbarNav">
        <ul class="navbar-nav ms-auto">
          <li class="nav-item">
            <a class="nav-link" href="/"><i class="fas fa-home"></i> Home</a>
          </li>
          <li class="nav-item">
            <a class="nav-link active" href="/update"><i class="fas fa-upload"></i> OTA Update</a>
          </li>
        </ul>
      </div>
    </div>
  </nav>

  <!-- Update Card -->
  <div class="container">
    <div class="card shadow-2-strong">
      <div class="card-body text-center">
        <h5 class="card-title"><i class="fas fa-upload"></i> OTA Firmware Update</h5>
        <form method="POST" action="/update" enctype="multipart/form-data" class="mt-4">
          <input type="file" name="firmware" class="form-control mb-3" required>
          <input type="submit" value="Upload" class="btn btn-success">
        </form>
      </div>
    </div>
  </div>
</body>
</html>
)rawliteral";