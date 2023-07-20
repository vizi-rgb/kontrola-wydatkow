window.onload = init;

function init() {
    document.getElementById("money").addEventListener("input", onMoneyInputChange);
}


function onMoneyInputChange() {
    input = document.getElementById("money").value;

    if (input != "") {
        document.getElementById("btn-y").style.visibility = "visible";
        document.getElementById("btn-n").style.visibility = "visible";
    } else {
        document.getElementById("btn-y").style.visibility = "hidden";
        document.getElementById("btn-n").style.visibility = "hidden";
    }
}