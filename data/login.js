var errorBackGroundColor = "#FA8072"; ///red color for incorrect input
/**
 * @brief submit function for the form in login.html
 * @param e Html Event
 */
function submit(e) {
    e.preventDefault(); ///prevent from submitting the form
    ///get inputs value
    var Ssid = e.target[0];
    var Password = e.target[1];
    ///init the error counter
    var Error = 0;
    ///check inputs validity
    if (!checkInput(Ssid.value)) {
        Ssid.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    if (!checkInput(Password.value)) {
        Password.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    ///If all inputs are correct then we post the request
    if (Error == 0) {
        ///Post request
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
            ///if we get a response
            console.log(response.statusText);
            ///We go to the next page if the login information were correct.
            if (response.statusText == "Created") {
                history.pushState({}, null, "/"); ///add next location to history
                location.reload(); ///reload the page
            }
            ///We put login information in red if incorect
            else {
                Ssid.style.backgroundColor = errorBackGroundColor;
                Password.style.backgroundColor = errorBackGroundColor;
            }
        });
    }
}
/**
 * @brief check the validity of an HTML input
 * @param t value of the input
 * @returns true if valid else false
 */
function checkInput(t) {
    return (t.length > 0);
}
//# sourceMappingURL=login.js.map