var gateway = "ws://squad1063:100/ws";
var websocket;
function InitWS() {
    console.log("Opening a webSocket");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
function onOpen(e) {
    console.log("Connection has been opened");
}
function onClose(e) {
    console.log("Connection has been closed");
}
function onMessage(e) {
    console.log("Received message from WebSocket");
    console.log(e.data);
}
function onLoad(e) {
    InitWS();
}
function sendMessage(e) {
    e.preventDefault();
    var elem = e.target;
    var toSend;
    switch (elem.id) {
        case "right-off":
            toSend = {
                motorB: {
                    state: "off",
                }
            };
            break;
        case "right-on":
            toSend = {
                motorB: {
                    state: "on",
                }
            };
            break;
        case "left-off":
            toSend = {
                motorA: {
                    state: "off",
                },
            };
            break;
        case "left-on":
            toSend = {
                motorA: {
                    state: "on",
                },
            };
            break;
        case "right-motor-speed":
            toSend = {
                motorB: {
                    speed: elem.value,
                },
            };
            break;
        case "left-motor-speed":
            toSend = {
                motorA: {
                    speed: elem.value,
                },
            };
            break;
        default:
            break;
    }
    console.log("Sending message");
    websocket.send(JSON.stringify(toSend));
}
//# sourceMappingURL=index.js.map