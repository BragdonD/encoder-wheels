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
    e.preventDefault();
    let elem : HTMLElement = <HTMLElement>e.target;
    let toSend : Object;
    
    switch (elem.id) {
        case "right-off":
            toSend = {
                motorB: {
                    state: "off",
                }
            }
            break;
        case "right-on":
            toSend = {
                motorB: {
                    state: "on",
                }
            }
            break;
        case "left-off":
            toSend = {
                motorA: {
                    state: "off",
                },
            }
            break;
        case "left-on":
            toSend = {
                motorA: {
                    state: "on",
                },
            }
            break;
        case "right-motor-speed":
            toSend = {
                motorB: {
                    speed: (<HTMLInputElement>elem).value,
                },
            }
            break;
        case "left-motor-speed":
            toSend = {
                motorA: {
                    speed: (<HTMLInputElement>elem).value,
                },
            }
            break;
    
        default:
            break;
    }
    console.log("Sending message");
    websocket.send(JSON.stringify(toSend));
}