<?php
session_start();
if ($_SESSION['role'] !== 'admin') {
    header("location: index.php");
    exit;
}
?>

<!DOCTYPE html>
<html lang="ru">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Админ-меню</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            margin: 0;
            padding: 0;
        }

        a {
            display: block;
            width: 200px;
            padding: 10px;
            margin: 10px;
            text-align: center;
            background-color: #3498db;
            color: white;
            text-decoration: none;
            border-radius: 5px;
        }

        a:hover {
            background-color: #2980b9;
        }
    </style>
</head>

<body>
    <a href="main.php">Основное</a>
    <a href="history_form.php">Отчеты</a>
    <a href="errors_form.php">Ошибки</a>
    <a href="operator_list.php">Операторы</a>
    <a href="logout.php">Logout</a>
</body>

</html>