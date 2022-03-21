var gateway = "ws://squad1063.local:100/ws";
var websocket;
var valuesB = [0];
var valuesA = [0];
var state = false;
var state2 = false;
var toggleBtn = false;
var toggleBtn2 = false;
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
    //console.log("Received message from WebSocket");
    var data = JSON.parse(e.data);
    //console.log(data);
    if (data["motorA"] !== undefined) {
        if (data["motorA"]["speed"] !== undefined) {
            var range = document.getElementById("left-motor-speed");
            range.value = data["motorA"]["speed"];
            var value = document.getElementById("left-range-value");
            value.innerHTML = data["motorA"]["speed"] + " turn/s";
        }
        if (data["motorA"]["state"] !== undefined) {
            if ((data["motorA"]["state"] === "on" && state === false) || (data["motorA"]["state"] === "off" && state === true)) {
                toggle(e);
            }
        }
    }
    if (data["motorB"] !== undefined) {
        if (data["motorB"]["speed"] !== undefined) {
            var range = document.getElementById("right-motor-speed");
            range.value = data["motorB"]["speed"];
            var value = document.getElementById("right-range-value");
            value.innerHTML = data["motorB"]["speed"] + " turn/s";
        }
        if (data["motorB"]["state"] !== undefined) {
            if ((data["motorB"]["state"] === "on" && state2 === false) || (data["motorB"]["state"] === "off" && state2 === true)) {
                toggle2(e);
            }
        }
    }
    if (data["CurrentSpeedB"] !== undefined) {
        if (valuesB.length > 20) {
            valuesB.shift();
        }
        valuesB.push(data["CurrentSpeedB"]);
    }
    if (data["CurrentSpeedA"] !== undefined) {
        if (valuesA.length > 20) {
            valuesA.shift();
        }
        valuesA.push(data["CurrentSpeedA"]);
        //console.log(valuesA);  
    }
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
    //console.log("Sending message");
    websocket.send(JSON.stringify(toSend));
}
function toggle(e) {
    var leftDivButtonContainer = document.getElementById("left-input-container");
    state = !state;
    if (state) {
        leftDivButtonContainer.classList.remove("off");
        leftDivButtonContainer.classList.add("on");
    }
    else {
        (document.getElementById("left-motor-speed")).value = "0";
        updateLeftRangeValue(0);
        leftDivButtonContainer.classList.add("off");
        leftDivButtonContainer.classList.remove("on");
    }
}
function toggle2(e) {
    var rightDivButtonContainer = document.getElementById("right-input-container");
    state2 = !state2;
    if (state2) {
        rightDivButtonContainer.classList.remove("off");
        rightDivButtonContainer.classList.add("on");
    }
    else {
        (document.getElementById("right-motor-speed")).value = "0";
        updateRightRangeValue(0);
        rightDivButtonContainer.classList.add("off");
        rightDivButtonContainer.classList.remove("on");
    }
}
var onChangeLeft = function (e) {
    sendMessage(e);
    updateLeftRangeValue(parseInt(e.target.value));
};
var updateLeftRangeValue = function (value) {
    document.getElementById("left-range-value").innerHTML = value + " turn/s";
};
var onChangeRight = function (e) {
    sendMessage(e);
    updateRightRangeValue(parseInt(e.target.value));
};
var updateRightRangeValue = function (value) {
    document.getElementById("right-range-value").innerHTML = value + " turn/s";
};
function toggle3(e) {
    var leftDivButtonToggle = document.getElementById("toggle-left-direction");
    toggleBtn = !toggleBtn;
    if (toggleBtn) {
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
    }));
}
function toggle4(e) {
    var rightDivButtonToggle = document.getElementById("toggle-right-direction");
    toggleBtn2 = !toggleBtn2;
    if (toggleBtn2) {
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
    }));
}
//# sourceMappingURL=index.js.map