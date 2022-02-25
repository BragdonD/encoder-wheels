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
    console.log("Sending message");
    websocket.send(JSON.stringify({ case: "ok" }));
}
//# sourceMappingURL=index.js.map