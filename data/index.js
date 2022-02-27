var gateway = "ws://squad1063.local:100/ws";
var websocket;
var state = false;
var state2 = false;
var toggleBtn = true;
var toggleBtn2 = true;
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
    var data = JSON.parse(e.data);
    if (data["motorA"] !== undefined) {
        if (data["motorA"]["speed"] !== undefined) {
            var range = document.getElementById("left-motor-speed");
            range.value = data["motorA"]["speed"];
            var value = document.getElementById("left-range-value");
            value.innerHTML = data["motorA"]["speed"] + " km/h";
        }
        else if (data["motorA"]["state"] !== undefined) {
            if ((data["motorA"]["state"] === "on" && state === false) || (data["motorA"]["state"] === "off" && state === true)) {
                toggle(e);
            }
        }
    }
    else if (data["motorB"] !== undefined) {
        if (data["motorB"]["speed"] !== undefined) {
            var range = document.getElementById("right-motor-speed");
            range.value = data["motorB"]["speed"];
            var value = document.getElementById("right-range-value");
            value.innerHTML = data["motorB"]["speed"] + " km/h";
        }
        else if (data["motorB"]["state"] !== undefined) {
            if ((data["motorB"]["state"] === "on" && state2 === false) || (data["motorB"]["state"] === "off" && state2 === true)) {
                toggle2(e);
            }
        }
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
    console.log("Sending message");
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
    document.getElementById("left-range-value").innerHTML = value + " km/h";
};
var onChangeRight = function (e) {
    sendMessage(e);
    updateRightRangeValue(parseInt(e.target.value));
};
var updateRightRangeValue = function (value) {
    document.getElementById("right-range-value").innerHTML = value + " km/h";
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
}
//# sourceMappingURL=index.js.map