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
      margin-top: 2rem;
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
  </div>

</body>
</html>
)rawliteral";