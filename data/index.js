var gateway = "ws://squad1063.local:100/ws"; ///websocket connection adress
var websocket; ///the websocket
/**
 * Variables to stores the last 20 motors speed value
 */
var valuesB = [0];
var valuesA = [0];
/**
 * Variables to stores the state of the differents buttons
 */
var state = false;
var state2 = false;
var state3 = false;
var toggleBtn = true;
var toggleBtn2 = true;
var toggleBtn3 = true;
/**
 * @brief function to init webSocekt
 */
function InitWS() {
    console.log("Opening a webSocket");
    websocket = new WebSocket(gateway);
    ///Setting up the function
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
/**
 * @brief Function to be called when the websocket connection is opened
 * @param e Open Event
 */
function onOpen(e) {
    console.log("Connection has been opened");
}
/**
 * @brief Function to be called when the websocket connection is closed
 * @param e Close Event
 */
function onClose(e) {
    console.log("Connection has been closed");
}
/**
 * @brief Function to be called when the websocket received a message
 * @param e Message Event
 */
function onMessage(e) {
    var data = JSON.parse(e.data); ///parse the received message to extract the data
    if (data["motorA"] !== undefined) { ///set data for motor A
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
    if (data["motorB"] !== undefined) { ///set data for motor B
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
    if (data["CurrentSpeedB"] !== undefined) { ///set data for values B
        if (valuesB.length > 100) {
            valuesB.shift();
        }
        valuesB.push(data["CurrentSpeedB"]);
    }
    if (data["CurrentSpeedA"] !== undefined) { ///set data for values A
        if (valuesA.length > 100) {
            valuesA.shift();
        }
        valuesA.push(data["CurrentSpeedA"]);
    }
}
/**
 * @brief Function to be called when the page is load
 * @param e
 */
function onLoad(e) {
    InitWS();
}
/**
 * @brief Function to send data to the websocket server
 * @param e HTLL Event
 */
function sendMessage(e) {
    e.preventDefault();
    ///Get the event target
    var elem = e.target;
    var toSend;
    ///Create the data to send in function of the event target id
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
    ///Send the data
    websocket.send(JSON.stringify(toSend));
}
function toggle(e) {
    var leftDivButtonContainer = document.getElementById("left-input-container");
    state = !state;
    if (state) {
        (document.getElementById("left-motor-speed")).disabled = false;
        leftDivButtonContainer.classList.remove("off");
        leftDivButtonContainer.classList.add("on");
    }
    else {
        (document.getElementById("left-motor-speed")).value = "0";
        updateLeftRangeValue(0);
        (document.getElementById("left-motor-speed")).disabled = true;
        leftDivButtonContainer.classList.add("off");
        leftDivButtonContainer.classList.remove("on");
    }
}
function toggle2(e) {
    var rightDivButtonContainer = document.getElementById("right-input-container");
    state2 = !state2;
    if (state2) {
        (document.getElementById("right-motor-speed")).disabled = false;
        rightDivButtonContainer.classList.remove("off");
        rightDivButtonContainer.classList.add("on");
    }
    else {
        (document.getElementById("right-motor-speed")).value = "0";
        updateRightRangeValue(0);
        (document.getElementById("right-motor-speed")).disabled = true;
        rightDivButtonContainer.classList.add("off");
        rightDivButtonContainer.classList.remove("on");
    }
}
function toggle5(e) {
    var DivButtonToggle = document.getElementById("both-input-container");
    state3 = !state3;
    if (state3) {
        (document.getElementById("motors-speed")).disabled = false;
        DivButtonToggle.classList.remove("off");
        DivButtonToggle.classList.add("on");
    }
    else {
        (document.getElementById("motors-speed")).disabled = true;
        (document.getElementById("motors-speed")).value = "0";
        updateBothRangeValue(0);
        DivButtonToggle.classList.add("off");
        DivButtonToggle.classList.remove("on");
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
var onChangeBoth = function (e) {
    sendMessage(e);
    updateRightRangeValue(parseInt(e.target.value));
};
var updateBothRangeValue = function (value) {
    document.getElementById("both-range-value").innerHTML = value + " turn/s";
};
function toggle6(e) {
    var DivButtonToggle = document.getElementById("toggle-both-direction");
    toggleBtn3 = !toggleBtn3;
    if (toggleBtn3) {
        DivButtonToggle.classList.remove("backwards");
        DivButtonToggle.classList.add("forwards");
        DivButtonToggle.innerHTML = "FORWARDS";
    }
    else {
        DivButtonToggle.classList.add("backwards");
        DivButtonToggle.classList.remove("forwards");
        DivButtonToggle.innerHTML = "BACKWARDS";
    }
    websocket.send(JSON.stringify({
        motorB: {
            direction: toggleBtn2 !== true ? "backwards" : "forwards"
        },
        motorA: {
            direction: toggleBtn2 !== true ? "backwards" : "forwards"
        }
    }));
}
//# sourceMappingURL=index.js.map