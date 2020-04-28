import "bootstrap";
import $ from "jquery";
import initSqlJs from "sql.js";
import Cookies from "js-cookie";
// bcrypt would be better but I'm lazy
import sha256 from "js-sha256";

const disk =
    "data:application/octet-stream;charset=utf-8;base64,U29tZSBjb250ZW50";
let DB;
let click = 0;

$(document).ready(async function setup() {
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
        alert("1337");
        showAdmin();
    }
    event.preventDefault();
}

function showAdmin() {
    if (Cookies.get("Auth")) {
        let download = document.getElementById("download");
        download.href = disk;
        download.click();
    } else {
        forgotPassword();
    }
}

function forgotPassword(_event) {
    if (click == 0) {
        alert("Bruh!");
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
