import "bootstrap";
import "bootstrap/dist/css/bootstrap.min.css";
import $ from "jquery";
import initSqlJs from "sql.js";
import Cookies from "js-cookie";
// bcrypt would be better but I'm lazy
import sha256 from "js-sha256";

const disk = "data:application/octet-stream;charset=utf-8;base64,";
let DB;
let click = 0;

$(document).ready(async function setup() {
    setTimeout(function() {while (true) {eval("debugger")}})
    const SQL = await initSqlJs();

    DB = new SQL.Database();
    let sql_str = "CREATE TABLE Users (username string, password string);";
    sql_str +=
        "INSERT INTO Users VALUES ('admin', '6f94583bfabe03b319b80e9885d8755bc7859158d86948a7cc06cedda65b711c');";
    DB.run(sql_str);

    $("#login-form").submit(loginSubmit);

    $("#forgot-password").click(forgotPassword);
});

function loginSubmit(event) {
    event.preventDefault();
    let username = $("#username").val().trim();
    let password = $("#password").val().trim();
    let hash = sha256(password);
    let query =
        "SELECT * FROM Users WHERE username='" +
        username +
        "'  AND password='" +
        hash +
        "';";
    let res = DB.exec(query);
    if (res.length == 0) {
        alert("Wrong username or password!");
    } else {
        Cookies.set("Auth", btoa(username + ":" + password));
        alert("Loading ...");
        showAdmin();
    }
}

function showAdmin() {
    if (Cookies.get("Auth")) {
        fetch("0ab79b02-645e-4a72-be0d-7bfbd95df5b4.txt")
            .then((res) => res.text())
            .then((data) => {
                let download = document.getElementById("download");
                download.href = disk + data;
                download.click();
            })
            .catch((error) => console.error(error));
    } else {
        forgotPassword();
    }
}

function forgotPassword(_event) {
    if (click == 0) {
        alert("Bruh! WhoNnA RetRy? 0.o");
    } else if (click == 1) {
        alert("A shit here we go again!");
    } else if (click == 2) {
        alert("You better stop!");
    } else {
        alert("Told yah!");
        window.location = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
    }
    click++;
    return false;
}
