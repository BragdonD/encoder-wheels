let errorBackGroundColor = "#FA8072";

function submit (e : Event) : void {
    e.preventDefault();

    let Ssid : HTMLInputElement = (<HTMLInputElement>e.target[0]);
    let Password : HTMLInputElement = (<HTMLInputElement>e.target[1]);

    let Error : number = 0;

    if ( !checkInput( Ssid.value ) ) {
        Ssid.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }

    if ( !checkInput( Password.value ) ) {
        Password.style.backgroundColor = errorBackGroundColor;
        Error += 1;
    }
    
    if ( Error == 0 ) {
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
            console.log(response.statusText);
            
            if( response.statusText == "Created" ) {
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

function checkInput (t : string) : boolean {
    return ( t.length > 0);
}