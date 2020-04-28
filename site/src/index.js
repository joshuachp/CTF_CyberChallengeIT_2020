import "bootstrap";
import $ from "jquery";
import initSqlJs from "sql.js";

let db;
$(document).ready(async function setup() {
    const SQL = await initSqlJs();

    // Create a database
    db = new SQL.Database();
    let sql_str = "CREATE TABLE Users (username string, password string);";
    sql_str += "INSERT INTO Users VALUES ('admin', 'password');";
    db.run(sql_str); // Run the query without returning anything
    console.log("Done");
});
