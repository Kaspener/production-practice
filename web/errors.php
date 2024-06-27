<?php
session_start();
if ($_SESSION['role'] !== 'admin') {
    header("location: index.php");
    exit;
}
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['start_datetime'], $_POST['end_datetime'])) {
        $start_datetime = date("Y-m-d H:i:s", strtotime($_POST['start_datetime']));
        $end_datetime = date("Y-m-d H:i:s", strtotime($_POST['end_datetime']));
        $device_number = isset($_POST['device_number']) ? $_POST['device_number'] : null;
        $terminal_number = isset($_POST['terminal_number']) ? $_POST['terminal_number'] : null;
        $installation_address = isset($_POST['installation_address']) ? $_POST['installation_address'] : null;
        $error_description = isset($_POST['error_description']) ? $_POST['error_description'] : null;
        $state = $_POST['state'] == "no" ? null : $_POST['state'];

        require_once 'config.php';
        $host = DB_HOST;
        $user = DB_USER;
        $pass = DB_PASS;
        $db_name = DB_NAME;
        $db_table = "errors";
        $link = mysqli_connect($host, $user, $pass, $db_name);

        if (!$link) {
            echo 'Не могу соединиться с БД. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
            exit;
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ошибки</title>
    <style>
        #operationtable {
            font-family: Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
            margin-top: 20px;
        }

        #operationtable td,
        #operationtable th {
            border: 1px solid #ddd;
            padding: 8px;
        }

        #operationtable tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        #operationtable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #04AA6D;
            color: white;
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

        .status-circle {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            display: inline-block;
            margin-right: 5px;
        }

        .red {
            background-color: red;
        }

        .green {
            background-color: green;
        }
    </style>
</head>

<body>
    <a href="errors_form.php" class="back-link"><span>&#8592;</span> Вернуться</a>
    <h1 style="text-align: center; color: #04AA6D; font-family: Arial, sans-serif; margin-top: 20px;">Ошибки</h1>
    <table id="operationtable">
        <thead>
            <tr>
                <th>Номер торгового аппарата</th>
                <th>Серийный номер платежного терминала</th>
                <th>Адрес установки торгового аппарата</th>
                <th>Состояние</th>
                <th>Описание ошибки</th>
                <th>Время устранения ошибки</th>
            </tr>
        </thead>
        <tbody>
            <?php
            $sql = "SELECT * FROM $db_table WHERE `startError` BETWEEN '$start_datetime' AND '$end_datetime' ORDER BY `ID` DESC";
            if ($device_number)
                $sql .= " AND `numberOfDevice` = $device_number";
            if ($terminal_number)
                $sql .= " AND `serial` = '$terminal_number'";
            if ($installation_address)
                $sql .= " AND `address` = '$installation_address'";
            if ($error_description)
                $sql .= " AND `error` = '$error_description'";
            if ($state == "1") {
                $sql .= " AND `endError` is not NULL";
            } else if ($state == "0") {
                $sql .= " AND `endError` is NULL";
            }
            $sql = mysqli_query($link, $sql);
            while ($result = mysqli_fetch_array($sql)) {
                echo "<tr><td>{$result['numberOfDevice']}</td>" .
                    "<td>{$result['serial']}</td>" .
                    "<td>{$result['address']}</td>" .
                    "<td>" .
                    '<div class="status-circle ' . ($result['endError'] == NULL ? 'red' : 'green') . '"></div>' . 
                    "{$result['startError']}</td>" .
                    "<td>{$result['error']}</td>";
                echo $result['endError'] == null ? "<td>-</td>" : "<td>{$result['endError']}</td>";
            }
            ?>
        </tbody>
    </table>
</body>

</html>