<?php
session_start();
if ($_SESSION['role'] !== 'admin') {
    header("location: index.php");
    exit;
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ошибки</title>
    <style>
        h1 {
            color: #04AA6D;
            text-align: center;
        }

        form {
            margin: 20px auto;
            max-width: 600px;
            padding: 20px;
            border: 1px solid #ddd;
            border-radius: 5px;
        }

        label {
            display: block;
            margin-bottom: 10px;
        }

        input[type="datetime-local"],
        input[type="text"],
        button {
            width: 100%;
            padding: 8px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 5px;
            box-sizing: border-box;
        }

        button {
            background-color: #04AA6D;
            color: white;
            cursor: pointer;
            transition: background-color 0.3s ease;
        }

        button:hover {
            background-color: #21825c;
        }

        .back-link {
            position: absolute;
            top: 10px;
            left: 10px;
            display: flex;
            align-items: center;
            text-decoration: none;
            color: #333;
            font-size: 16px;
            text-transform: uppercase;
        }

        .back-link span {
            margin-right: 5px;
        }

        .back-link:hover {
            color: #04AA6D;
        }

        select {
            appearance: none;

            width: 100%;
            max-width: 200px;
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 5px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            background-color: white;
            cursor: pointer;
        }

        select:focus {
            outline: none;
            border-color: #333;
            box-shadow: 0 0 5px rgba(0, 0, 0, 0.5);
        }

        option[selected] {
            background-color: #f0f0f0;
            color: black;
        }
    </style>

</head>

<body>
    <a href="admin_menu.php" class="back-link"><span>&#8592;</span> Меню</a>
    <h1>Ошибки</h1>
    <form id="reportForm" action="errors.php" method="post">
        <label for="start_datetime">Начало периода:</label>
        <input type="datetime-local" id="start_datetime" name="start_datetime">
        <label for="end_datetime">Конец периода:</label>
        <input type="datetime-local" id="end_datetime" name="end_datetime">
        <label for="device_number">Номер торгового аппарата:</label>
        <input type="text" id="device_number" name="device_number" placeholder="Не обязательно к заполнению">
        <label for="terminal_number">Номер терминала:</label>
        <input type="text" id="terminal_number" name="terminal_number" placeholder="Не обязательно к заполнению">
        <label for="installation_address">Адрес:</label>
        <input type="text" id="installation_address" name="installation_address"
            placeholder="Не обязательно к заполнению">
        <label for="state">Состояние:</label>
        <select id="state" name="state">
            <option value="no">Не выбрано</option>
            <option value="1">Работает</option>
            <option value="0">Ошибка</option>
        </select>
        <!-- <input type="text" id="state" name="state" placeholder="Не обязательно к заполнению"> -->
        <label for="error_description">Описание ошибки:</label>
        <input type="text" id="error_description" name="error_description" placeholder="Не обязательно к заполнению">
        <button type="submit">Показать ошибки</button>
    </form>
</body>

<script>
    var startDate = new Date();
    startDate.setFullYear(startDate.getFullYear() - 1);
    document.getElementById('start_datetime').value = startDate.toISOString().slice(0, 16);

    var endDate = new Date();
    endDate.setHours(endDate.getHours() + 7);
    document.getElementById('end_datetime').value = endDate.toISOString().slice(0, 16);
</script>

</html>