const errorBackGroundColor : string = "#FA8072"; ///red color for incorrect input

/**
 * @brief submit function for the form in login.html
 * @param e Html Event
 */
function submit (e : Event) : void {
    e.preventDefault(); ///prevent from submitting the form

    ///get inputs value
    let Ssid : HTMLInputElement = (<HTMLInputElement>e.target[0]);
    let Password : HTMLInputElement = (<HTMLInputElement>e.target[1]);

    ///init the error counter
    let Error : number = 0;

    ///check inputs validity
    if ( !checkInput( Ssid.value ) ) {
        Ssid.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }

    if ( !checkInput( Password.value ) ) {
        Password.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    
    ///If all inputs are correct then we post the request
    if ( Error == 0 ) {
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
        }).then(response => {
            ///if we get a response
            console.log(response.statusText);
            ///We go to the next page if the login information were correct.
            if( response.statusText == "Created" ) {
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
function checkInput (t : string) : boolean {
    return ( t.length > 0);
}