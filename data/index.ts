const gateway : string = "ws://squad1063.local:100/ws";
let websocket : WebSocket;

let valuesB = [0];
let valuesA = [0];

let state : boolean = false;
let state2 : boolean = false;
let toggleBtn : boolean = false;
let toggleBtn2 : boolean = false;

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
    //console.log("Received message from WebSocket");
    let data : Object = JSON.parse(e.data);

    //console.log(data);
    
    
    if (data["motorA"] !== undefined) {
        if (data["motorA"]["speed"] !== undefined) {
            let range : HTMLElement = document.getElementById("left-motor-speed");
            (<HTMLInputElement>range).value = data["motorA"]["speed"];
            let value : HTMLElement = document.getElementById("left-range-value");
            value.innerHTML = data["motorA"]["speed"] + " turn/s";
        }
        if(data["motorA"]["state"] !== undefined){
            if( (data["motorA"]["state"] === "on" && state === false) || (data["motorA"]["state"] === "off" && state === true)) {
                toggle(e);
            }
        }
    }
    if (data["motorB"] !== undefined) {
        if (data["motorB"]["speed"] !== undefined) {
            let range : HTMLElement = document.getElementById("right-motor-speed");
            (<HTMLInputElement>range).value = data["motorB"]["speed"];
            let value : HTMLElement = document.getElementById("right-range-value");
            value.innerHTML = data["motorB"]["speed"] + " turn/s";
        }
        if(data["motorB"]["state"] !== undefined){
            if( (data["motorB"]["state"] === "on" && state2 === false) || (data["motorB"]["state"] === "off" && state2 === true)) {
                toggle2(e);
            }
        }
    }
    if(data["CurrentSpeedB"] !== undefined) {
        if(valuesB.length > 20) {
            valuesB.shift();
        }
        valuesB.push(data["CurrentSpeedB"]);
    }
    if(data["CurrentSpeedA"] !== undefined) {
        if(valuesA.length > 20) {
            valuesA.shift();
        }
        valuesA.push(data["CurrentSpeedA"]);
        //console.log(valuesA);  
    }
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
    //console.log("Sending message");
    websocket.send(JSON.stringify(toSend));
}

function toggle(e : Event) : void {
    let leftDivButtonContainer = document.getElementById("left-input-container");

    state = !state;

    if ( state ) {
        leftDivButtonContainer.classList.remove("off");
        leftDivButtonContainer.classList.add("on");
    }
    else {
        (<HTMLInputElement>(document.getElementById("left-motor-speed"))).value = "0";
        updateLeftRangeValue(0);
        leftDivButtonContainer.classList.add("off");
        leftDivButtonContainer.classList.remove("on");
    }
}

function toggle2(e : Event) : void {
    let rightDivButtonContainer = document.getElementById("right-input-container");

    state2 = !state2;
    if ( state2 ) {
        rightDivButtonContainer.classList.remove("off");
        rightDivButtonContainer.classList.add("on");
    }
    else {
        (<HTMLInputElement>(document.getElementById("right-motor-speed"))).value = "0";
        updateRightRangeValue(0);
        rightDivButtonContainer.classList.add("off");
        rightDivButtonContainer.classList.remove("on");
    }
}
                        
const onChangeLeft = (e : Event) => {
    sendMessage(e);
    updateLeftRangeValue(parseInt((<HTMLInputElement>e.target).value));
}

const updateLeftRangeValue = (value : number) => {
    document.getElementById("left-range-value").innerHTML = value + " turn/s";
};

const onChangeRight = (e : Event) => {
    sendMessage(e);
    updateRightRangeValue(parseInt((<HTMLInputElement>e.target).value));
}

const updateRightRangeValue = (value : number) => {
    document.getElementById("right-range-value").innerHTML = value + " turn/s";
};

function toggle3(e : Event) : void {
    let leftDivButtonToggle = document.getElementById("toggle-left-direction");

    toggleBtn = !toggleBtn;

    if ( toggleBtn ) {
        leftDivButtonToggle.classList.remove("backwards");
        leftDivButtonToggle.classList.add("forwards");
        leftDivButtonToggle.innerHTML = "FORWARDS";
    }
    else {
        leftDivButtonToggle.classList.add("backwards");
        leftDivButtonToggle.classList.remove("forwards");
        leftDivButtonToggle.innerHTML = "BACKWARDS";
    }

    websocket.send(JSON.stringify({
        motorA: {
            direction: toggleBtn !== true ? "backwards" : "forwards"
        }
    }))
}

function toggle4(e : Event) : void {
    let rightDivButtonToggle = document.getElementById("toggle-right-direction");

    toggleBtn2 = !toggleBtn2;

    if ( toggleBtn2 ) {
        rightDivButtonToggle.classList.remove("backwards");
        rightDivButtonToggle.classList.add("forwards");
        rightDivButtonToggle.innerHTML = "FORWARDS";
    }
    else {
        rightDivButtonToggle.classList.add("backwards");
        rightDivButtonToggle.classList.remove("forwards");
        rightDivButtonToggle.innerHTML = "BACKWARDS";
    }

    websocket.send(JSON.stringify({
        motorB: {
            direction: toggleBtn2 !== true ? "backwards" : "forwards"
        }
    }))
}