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
var state = true;
var state2 = true;
var state3 = true;
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
    document.getElementById("status").innerHTML = "Online";
    document.getElementById("status").classList.add("online");
    websocket.send(JSON.stringify({
        newConnection: true,
    }));
}
/**
 * @brief Function to be called when the websocket connection is closed
 * @param e Close Event
 */
function onClose(e) {
    console.log("Connection has been closed");
    document.getElementById("status").innerHTML = "Offline";
    document.getElementById("status").classList.add("offline");
}
/**
 * @brief Function to be called when the websocket received a message
 * @param e Message Event
 */
function onMessage(e) {
    var data = JSON.parse(e.data); ///parse the received message to extract the data
    if (data["motorA"] !== undefined && data["motorB"] !== undefined) {
        console.log(data);
        var range = document.getElementById("left-motor-speed");
        range.value = data["motorA"]["speed"];
        var value = document.getElementById("left-range-value");
        value.innerHTML = data["motorA"]["speed"] + " turn/s";
        if (data["motorA"]["state"] === 1) {
            var leftDivButtonContainer = document.getElementById("left-input-container");
            (document.getElementById("left-motor-speed")).disabled = false;
            leftDivButtonContainer.classList.remove("off");
            leftDivButtonContainer.classList.add("on");
            state = true;
        }
        else {
            var leftDivButtonContainer = document.getElementById("left-input-container");
            (document.getElementById("left-motor-speed")).disabled = true;
            leftDivButtonContainer.classList.add("off");
            leftDivButtonContainer.classList.remove("on");
            state = false;
        }
        var range2 = document.getElementById("right-motor-speed");
        range2.value = data["motorB"]["speed"];
        var value2 = document.getElementById("right-range-value");
        value2.innerHTML = data["motorB"]["speed"] + " turn/s";
        if (data["motorB"]["state"] === 1) {
            var rightDivButtonContainer = document.getElementById("right-input-container");
            (document.getElementById("right-motor-speed")).disabled = false;
            rightDivButtonContainer.classList.remove("off");
            rightDivButtonContainer.classList.add("on");
            state2 = true;
        }
        else {
            var rightDivButtonContainer = document.getElementById("right-input-container");
            (document.getElementById("right-motor-speed")).disabled = true;
            rightDivButtonContainer.classList.add("off");
            rightDivButtonContainer.classList.remove("on");
            state2 = false;
        }
    }
    else {
        if (data["motorA"] !== undefined) { ///set data for motor A
            if (data["motorA"]["speed"] !== undefined) {
                var range = document.getElementById("left-motor-speed");
                range.value = data["motorA"]["speed"];
                var value = document.getElementById("left-range-value");
                value.innerHTML = data["motorA"]["speed"] + " turn/s";
            }
            if (data["motorA"]["state"] !== undefined) {
                if ((data["motorA"]["state"] === "on" && state === true) || (data["motorA"]["state"] === "off" && state === false)) {
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
                if ((data["motorB"]["state"] === "on" && state2 === true) || (data["motorB"]["state"] === "off" && state2 === false)) {
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
        if (data["kp_a"] !== undefined) {
            //console.log(data);
            document.getElementById("left-kp").value = data["kp_a"];
            document.getElementById("left-kp-value").innerHTML = data["kp_a"];
        }
        if (data["ki_a"] !== undefined) {
            //console.log(data);
            document.getElementById("left-ki").value = data["ki_a"];
            document.getElementById("left-ki-value").innerHTML = data["ki_a"];
        }
        if (data["kd_a"] !== undefined) {
            //console.log(data);
            document.getElementById("left-kd").value = data["kd_a"];
            document.getElementById("left-kd-value").innerHTML = data["kd_a"];
        }
        if (data["kp_b"] !== undefined) {
            //console.log(data);
            document.getElementById("right-kp").value = data["kp_b"];
            document.getElementById("right-kp-value").innerHTML = data["kp_b"];
        }
        if (data["ki_b"] !== undefined) {
            //console.log(data);
            document.getElementById("right-ki").value = data["ki_b"];
            document.getElementById("right-ki-value").innerHTML = data["ki_b"];
        }
        if (data["kd_b"] !== undefined) {
            //console.log(data);
            document.getElementById("right-kd").value = data["kd_b"];
            document.getElementById("right-kd-value").innerHTML = data["kd_b"];
        }
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
    console.log(elem.id);
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
        case "both-off":
            toSend = {
                motorB: {
                    state: "off",
                },
                motorA: {
                    state: "off",
                },
            };
            break;
        case "both-on":
            toSend = {
                motorB: {
                    state: "on",
                },
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
        case "motors-speed":
            toSend = {
                motorA: {
                    speed: elem.value,
                },
                motorB: {
                    speed: elem.value,
                }
            };
            break;
        case "left-kp":
            toSend = {
                kp_a: elem.value
            };
            break;
        case "left-ki":
            toSend = {
                ki_a: elem.value
            };
            break;
        case "left-kd":
            toSend = {
                kd_a: elem.value
            };
            break;
        case "right-kp":
            toSend = {
                kp_b: elem.value
            };
            break;
        case "right-ki":
            toSend = {
                ki_b: elem.value
            };
            break;
        case "right-kd":
            toSend = {
                kd_b: elem.value
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
    state = !state;
}
function toggle2(e) {
    var rightDivButtonContainer = document.getElementById("right-input-container");
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
    state2 = !state2;
}
function toggle5(e) {
    var DivButtonToggle = document.getElementById("both-input-container");
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
    state3 = !state3;
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
    if (!toggleBtn) {
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
    toggleBtn = !toggleBtn;
}
function toggle4(e) {
    var rightDivButtonToggle = document.getElementById("toggle-right-direction");
    if (!toggleBtn2) {
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
    toggleBtn2 = !toggleBtn2;
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
    toggleBtn3 = !toggleBtn3;
}
//# sourceMappingURL=index.js.map