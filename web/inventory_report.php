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
        $type_of_product = isset($_POST['type_of_product']) ? $_POST['type_of_product'] : null;
        if ($type_of_product == "") {
            $type_of_product = "1, 2, 3, 4";
        }

        require_once 'config.php';
        $host = DB_HOST;
        $user = DB_USER;
        $pass = DB_PASS;
        $db_name = DB_NAME;
        $db_table = "sellAndLoadHistory";
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
    <title>Отчет по продажам</title>
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
    <h1 style="text-align: center; color: #04AA6D; font-family: Arial, sans-serif; margin-top: 20px;">Инвентаризационный
        отчет</h1>
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
            <button type="submit" formaction="maintenance_report.php" class="tab-button">Отчет по обслуживаю и простоям</button>
        </div>
    </form>
    <table id="operationtable">
        <thead>
            <tr>
                <th>Номер торгового аппарата</th>
                <th>Серийный номер платежного терминала</th>
                <th>Адрес установки торгового аппарата</th>
                <th>Товар</th>
                <th>Остаток на
                    <?php echo $start_datetime; ?>
                </th>
                <th>Количество добавленного товара</th>
                <th>Количество проданного товара</th>
                <th>Остаток на
                    <?php echo $end_datetime; ?>
                </th>
            </tr>
        </thead>
        <tbody>
            <?php
            $sql = "SELECT\n"
                . "    `numberOfDevice`, \n"
                . "    `serial`, \n"
                . "    `address`, \n"
                . "    SUM(CASE WHEN `isSell` = 0 THEN cell1 ELSE 0 END) AS `scell1`,\n"
                . "    SUM(CASE WHEN `isSell` = 1 THEN cell1 ELSE 0 END) AS `sellcell1`,\n"
                . "    SUM(CASE WHEN `isSell` = 0 THEN cell2 ELSE 0 END) AS `scell2`,\n"
                . "    SUM(CASE WHEN `isSell` = 1 THEN cell2 ELSE 0 END) AS `sellcell2`,\n"
                . "    SUM(CASE WHEN `isSell` = 0 THEN cell3 ELSE 0 END) AS `scell3`,\n"
                . "    SUM(CASE WHEN `isSell` = 1 THEN cell3 ELSE 0 END) AS `sellcell3`,\n"
                . "    SUM(CASE WHEN `isSell` = 0 THEN cell4 ELSE 0 END) AS `scell4`,\n"
                . "    SUM(CASE WHEN `isSell` = 1 THEN cell4 ELSE 0 END) AS `sellcell4`,\n"
                . "    MAX(`isSell`) as `isSell`\n"
                . "FROM\n"
                . "    `sellAndLoadHistory`\n"
                . "WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime'";
            $tempsql = "";
            if ($device_number)
                $tempsql .= " AND `numberOfDevice` = $device_number";
            if ($terminal_number)
                $tempsql .= " AND `serial` = '$terminal_number'";
            if ($installation_address)
                $tempsql .= " AND `address` = '$installation_address'";
            $sql .= $tempsql;
            $sql .= " GROUP BY `numberOfDevice`, `serial`, `address` ORDER BY `numberOfDevice`;";
            $types = explode(", ", $type_of_product);
            if (!$types) {
                echo "Ошибка ввода вида товара";
                exit;
            }
            $rowspan = count($types);
            $sql = mysqli_query($link, $sql);
            while ($result = mysqli_fetch_array($sql)) {
                echo "<tr><td rowspan=\"$rowspan\">{$result['numberOfDevice']}</td>" .
                    "<td rowspan=\"$rowspan\">{$result['serial']}</td>" .
                    "<td rowspan=\"$rowspan\">{$result['address']}</td>";
                if (in_array(1, $types)) {
                    $num = 1;
                    echo "<td>№{$num}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT (CASE WHEN `isSell` = 0 THEN `remains{$num}` - `cell{$num}` ELSE `remains{$num}` + `cell{$num}` END) AS `calculated_remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " LIMIT 1"))['calculated_remains'] . "</td>";
                    echo "<td>{$result['scell1']}</td>";
                    echo "<td>{$result['sellcell1']}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT `remains{$num}` as `remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " ORDER BY ID DESC LIMIT 1"))['remains'] . "</td></tr><tr>";
                }
                if (in_array(2, $types)) {
                    $num = 2;
                    echo "<td>№{$num}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT (CASE WHEN `isSell` = 0 THEN `remains{$num}` - `cell{$num}` ELSE `remains{$num}` + `cell{$num}` END) AS `calculated_remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " LIMIT 1"))['calculated_remains'] . "</td>";
                    echo "<td>{$result['scell2']}</td>";
                    echo "<td>{$result['sellcell2']}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT `remains{$num}` as `remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " ORDER BY ID DESC LIMIT 1"))['remains'] . "</td></tr><tr>";
                }
                if (in_array(3, $types)) {
                    $num = 3;
                    echo "<td>№{$num}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT (CASE WHEN `isSell` = 0 THEN `remains{$num}` - `cell{$num}` ELSE `remains{$num}` + `cell{$num}` END) AS `calculated_remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " LIMIT 1"))['calculated_remains'] . "</td>";
                    echo "<td>{$result['scell3']}</td>";
                    echo "<td>{$result['sellcell3']}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT `remains{$num}` as `remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " ORDER BY ID DESC LIMIT 1"))['remains'] . "</td></tr><tr>";
                }
                if (in_array(4, $types)) {
                    $num = 4;
                    echo "<td>№{$num}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT (CASE WHEN `isSell` = 0 THEN `remains{$num}` - `cell{$num}` ELSE `remains{$num}` + `cell{$num}` END) AS `calculated_remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " LIMIT 1"))['calculated_remains'] . "</td>";
                    echo "<td>{$result['scell4']}</td>";
                    echo "<td>{$result['sellcell4']}</td>";
                    echo "<td>" . mysqli_fetch_assoc(mysqli_query($link, "SELECT `remains{$num}` as `remains` FROM `sellAndLoadHistory` WHERE `datatime` BETWEEN '$start_datetime' AND '$end_datetime' AND `numberOfDevice` = {$result['numberOfDevice']}" . $tempsql . " ORDER BY ID DESC LIMIT 1"))['remains'] . "</td></tr><tr>";
                }
                echo "</tr>";
            }
            ?>
        </tbody>
    </table>
</body>

</html>