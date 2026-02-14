#ifndef WEBSITE_DATA
#define WEBSITE_DATA

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Daly BMS Dashboard</title>
    <style>
        body { font-family: sans-serif; background-color: #f4f7f6; margin: 0; padding: 20px; color: #333; }
        h1 { text-align: center; color: #2c3e50; }
        .container { 
            display: flex; 
            flex-wrap: wrap; 
            justify-content: center; 
            gap: 20px; 
            max-width: 1200px; 
            margin: 0 auto; 
        }
        .card { 
            background: white; 
            padding: 20px; 
            border-radius: 15px; 
            box-shadow: 0 4px 6px rgba(0,0,0,0.1); 
            min-width: 200px; 
            flex: 1 1 250px; 
            text-align: center;
        }
        .card h3 { margin-top: 0; color: #7f8c8d; font-size: 0.9rem; text-transform: uppercase; }
        .value { font-size: 1.8rem; font-weight: bold; color: #2980b9; }
        .unit { font-size: 1rem; color: #7f8c8d; }
        .status { color: #27ae60; font-weight: bold; }
    </style>
</head>
<body>
    <h1>Daly BMS Dashboard</h1>
    <div class="container" id="dashboard">
        <div class="card"><h3>Status</h3><div class="value" id="status" style="font-size:1.2rem">Connecting...</div></div>
        <div class="card"><h3>Pack Voltage</h3><div class="value" id="volts">--</div><span class="unit">V</span></div>
        <div class="card"><h3>Pack Current</h3><div class="value" id="amps">--</div><span class="unit">A</span></div>
        <div class="card"><h3>State of Charge</h3><div class="value" id="soc">--</div><span class="unit">%</span></div>
        <div class="card"><h3>Temp (Avg)</h3><div class="value" id="temp">--</div><span class="unit">°C</span></div>
        <div class="card"><h3>Cell Voltages</h3><div class="value" id="cellvolts">--</div><span class="unit">mV</span></div>
        <div class="card"><h3>Max Cell</h3><div class="value" id="maxcell">--</div><span class="unit">mV</span></div>
        <div class="card"><h3>Min Cell</h3><div class="value" id="mincell">--</div><span class="unit">mV</span></div>
    </div>

    <script>
      function setFields(values) {
        const fields = ['volts', 'amps', 'soc', 'temp', 'maxcell', 'mincell', 'status', 'cellvolts'];
        fields.forEach(field => {
            document.getElementById(field).innerText = values[field] ?? "-";
        });
      }

      function updateData() {
        fetch('/data')
            .then(response => response.json())
            .then(data => {
                if (data && Object.keys(data).length > 0) {
                    setFields({
                        volts: data.voltage,
                        amps: data.current,
                        soc: data.soc,
                        temp: data.temperatures.avg,
                        maxcell: data.cells.max_mv,
                        mincell: data.cells.min_mv,
                        status: data.status,
                        cellvolts: data.cells?.voltages?.join(', ') ?? "-",
                    });
                } else {
                    setFields({"status": "Not connected"});
                }
            })
            .catch(err => {
                console.error('Error fetching data:', err);
                setFields({});
            });
    }

    setInterval(updateData, 10000);
    updateData();
    </script>
</body>
</html>
)rawliteral";

#endif