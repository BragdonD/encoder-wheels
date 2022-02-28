"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var auto_1 = require("chart.js/auto");
var data = [];
setInterval(AddValue, 100);
var config = {
    type: "line",
    data: data
};
function AddValue() {
    data.push(document.getElementById("right-motor-speed").value);
}
var ctxLeft = document.getElementById("right-serial");
var LeftChart = new auto_1.default(ctxLeft, {
    data: data,
});
//# sourceMappingURL=serial.js.map