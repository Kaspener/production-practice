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
    <title>Отчет по обслуживанию и простоям</title>
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

        .tab-buttons {
            display: flex;
            justify-content: space-around;
            margin-bottom: 20px;
        }

        .tab-button {
            padding: 10px 20px;
            background-color: #f0f0f0;
            border: none;
            border-bottom: 3px solid transparent;
            cursor: pointer;
            transition: all 0.3s ease;
        }

        .tab-button:hover {
            background-color: #e0e0e0;
        }

        .tab-button.active {
            border-bottom-color: #04AA6D;
        }
    </style>
</head>

<body>
    <a href="history_form.php" class="back-link"><span>&#8592;</span> Вернуться</a>
    <h1 style="text-align: center; color: #04AA6D; font-family: Arial, sans-serif; margin-top: 20px;">Отчет по
        обслуживанию и простоям</h1>
    <form id="reportForm" method="post">
        <input type="hidden" id="start_datetime" name="start_datetime" value="<?= $start_datetime ?>">
        <input type="hidden" id="end_datetime" name="end_datetime" value="<?= $end_datetime ?>">
        <input type="hidden" id="device_number" name="device_number" value="<?= $device_number ?>">
        <input type="hidden" id="terminal_number" name="terminal_number" value="<?= $terminal_number ?>">
        <input type="hidden" id="installation_address" name="installation_address" value="<?= $installation_address ?>">
        <input type="hidden" id="type_of_product" name="type_of_product" value="<?= $type_of_product ?>">
        <input type="hidden" id="error_description" name="error_description" value="<?= $error_description ?>">
        <div class="tab-buttons">
            <button type="submit" formaction="operation_history_table.php" class="tab-button">История операций</button>
            <button type="submit" formaction="sell_report.php" class="tab-button">Отчет по продажам</button>
            <button type="submit" formaction="inventory_report.php" class="tab-button">Инвентаризационный отчет</button>
        </div>
    </form>
    <table id="operationtable">
        <thead>
            <tr>
                <th>Номер торгового аппарата</th>
                <th>Серийный номер платежного терминала</th>
                <th>Адрес установки торгового аппарата</th>
                <th>Описание ошибки</th>
                <th>Дата и время возникновения ошибки</th>
                <th>Дата и время исправления ошибки</th>
                <th>Время простоя</th>
            </tr>
        </thead>
        <tbody>
            <?php
            $sql = "SELECT * FROM $db_table WHERE `startError` BETWEEN '$start_datetime' AND '$end_datetime'";
            if ($device_number)
                $sql .= " AND `numberOfDevice` = $device_number";
            if ($terminal_number)
                $sql .= " AND `serial` = '$terminal_number'";
            if ($installation_address)
                $sql .= " AND `address` = '$installation_address'";
            if ($error_description)
                $sql .= " AND `error` = '$error_description'";
            $sql .= " ORDER BY `ID` DESC";
            $sql = mysqli_query($link, $sql);
            while ($result = mysqli_fetch_array($sql)) {
                echo "<tr><td>{$result['numberOfDevice']}</td>" .
                    "<td>{$result['serial']}</td>" .
                    "<td>{$result['address']}</td>" .
                    "<td>{$result['error']}</td>" .
                    "<td>{$result['startError']}</td>";
                echo $result['endError'] == null ? "<td>-</td>" : "<td>{$result['endError']}</td>";
                $date1 = new DateTime($result['startError']);
                if ($result['endError'] == null) {
                    $date2 = new DateTime();
                } else {
                    $date2 = new DateTime($result['endError']);
                }

                $interval = $date1->diff($date2);

                if ($interval->days > 0) {
                    $totalHours = $interval->days * 24 + $interval->h;
                    echo "<td>" . $totalHours . ":" . $interval->format('%I:%S') . "</td>";
                } else {
                    echo "<td>" . $interval->format('%H:%I:%S') . "</td>";
                }
            }
            ?>
        </tbody>
    </table>
</body>

</html>