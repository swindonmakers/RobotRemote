
static const char pageWebSocketMonitor[] = R"~(
<head>
    <title>Robot Remote - WebSocket Monitor</title>
</head>
<body>

<form onsubmit="return false;">
    <h3>Websocket Monitor</h3>
    <label>Host</label>
    <input type="text" id="host" value="ws://192.168.4.1:81/ws" style="width:160px;" placeholder="ws://192.168.4.1:81/ws"/>
    <input type="button" value="Connect" onclick="connect(host.value)" />
    <input type="button" value="Disconnect" onclick="disconnect()" />
    <br>
	<label>Connection Status:</label>
	<input type="text" id="connectionLabel" readonly value="Idle"/>
	<br />

    <br>
    <textarea id="output" style="width:100%;height:70%;"></textarea>
    <br>
    <input type="button" value="Clear" onclick="clearText()">
</form>

<script type="text/javascript">
    var output = document.getElementById('output');
    var connectionLabel = document.getElementById('connectionLabel');
	var socket;

	function connect(host) {
		console.log('connect', host);
		if (window.WebSocket) {
			connectionLabel.value = "Connecting";
			if(socket) {
				socket.close();
	            socket = null;
			}
			
	        socket = new WebSocket(host);
	
	        socket.onmessage = function(event) {
	            output.value += event.data; // + "\r\n";
            	var textarea = document.getElementById('output');
				textarea.scrollTop = textarea.scrollHeight;
	        };
	        socket.onopen = function(event) {
	            isRunning = true;
	            connectionLabel.value = "Connected";
	        };
	        socket.onclose = function(event) {
	            isRunning = false;
	            connectionLabel.value = "Disconnected";
	        };
	        socket.onerror = function(event) {
	            connectionLabel.value = "Error";
	        };
	    } else {
	        alert("Your browser does not support Web Socket.");
	    }
	}

	function disconnect() {
	    socket.close();
	    socket = null;
	}
		
    function send(data) {
        if (!window.WebSocket) {
            return;
        }

        if (socket.readyState == WebSocket.OPEN) {
            var message = data;
            output.value += 'sending : ' + data + '\r\n';
            socket.send(message);
        } else {
            alert("The socket is not open.");
        }
    }

    function clearText() {
        output.value="";
    }

</script>

</body>
</html>
)~";

