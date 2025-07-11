const char UPDATE_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>OTA Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" rel="stylesheet"/>
  <link href="https://cdnjs.cloudflare.com/ajax/libs/mdb-ui-kit/6.4.1/mdb.min.css" rel="stylesheet"/>
  <style>body{padding:2rem}</style>
</head>
<body class="container text-center">
  <h4><i class="fas fa-upload"></i> OTA Firmware Update</h4>
  <form method="POST" action="/update" enctype="multipart/form-data" class="mt-4">
    <input type="file" name="firmware" class="form-control mb-3" required>
    <input type="submit" value="Upload" class="btn btn-success">
  </form>
  <a href="/" class="btn btn-secondary mt-4"><i class="fas fa-arrow-left"></i> Back</a>
</body>
</html>
)rawliteral";