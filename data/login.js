var errorBackGroundColor = "#FA8072";
function submit(e) {
    e.preventDefault();
    var Ssid = e.target[0];
    var Password = e.target[1];
    var Error = 0;
    if (!checkInput(Ssid.value)) {
        Ssid.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    if (!checkInput(Password.value)) {
        Password.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    if (Error == 0) {
        fetch("/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Access-Control-Allow-Origin": "*",
                "Accept": "application/json",
            },
            body: JSON.stringify({
                ssid: Ssid.value,
                password: Password.value
            })
        }).then(function (response) {
            console.log(response.statusText);
            if (response.statusText == "Created") {
                history.pushState({}, null, "/");
                location.reload();
            }
            else {
                Ssid.style.backgroundColor = errorBackGroundColor;
                Password.style.backgroundColor = errorBackGroundColor;
            }
        });
    }
}
function checkInput(t) {
    return (t.length > 0);
}
//# sourceMappingURL=login.js.map