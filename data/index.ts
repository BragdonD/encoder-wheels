const gateway : string = "ws://squad1063:100/ws";
let websocket : WebSocket;

function InitWS() : void {
    console.log("Opening a webSocket");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(e : Event) : void {
    console.log("Connection has been opened");
}

function onClose(e : CloseEvent) : void {
    console.log("Connection has been closed");
}

function onMessage(e : MessageEvent) : void {
    console.log("Received message from WebSocket")
    console.log(e.data);
}

function onLoad(e : Event) : void {
    InitWS();
    
}

function sendMessage(e : Event) : void {
    console.log("Sending message");
    websocket.send(JSON.stringify({case: "ok"}));
}