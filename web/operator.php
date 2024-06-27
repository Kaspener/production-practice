<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Панель оператора</title>
    <style>
        #maintable {
            font-family: Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
            margin-top: 50px;
        }

        #maintable td,
        #maintable th {
            border: 1px solid #ddd;
            padding: 8px;
        }

        #maintable tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        #maintable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #04AA6D;
            color: white;
        }

        input[type="number"] {
            width: calc(100% - 20px);
            padding: 8px;
            border: 1px solid #ccc;
            border-radius: 4px;
            box-sizing: border-box;
            transition: border-color 0.3s;
        }

        input[type="number"]:focus {
            outline: none;
            border-color: #0066ff;
        }

        .submit-container {
            margin-top: 10px;
            text-align: center;
        }

        .submit-btn {
            background-color: #04AA6D;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
        }

        .submit-btn:hover {
            background-color: #056b43;
        }

        .logout-btn {
            background-color: #dc3545;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            font-size: 16px;
            text-decoration: none;
            transition: background-color 0.3s;
        }

        .logout-btn:hover {
            background-color: #c82333;
        }

        .logout-container {
            position: fixed;
            top: 20px;
            right: 20px;
        }
    </style>
    <script>
        function updateRemaining(input, number) {
            var insertCount = parseInt(input.value);
            var maxCount = parseInt(input.getAttribute('max'));
            var currentCount = parseInt(document.getElementById('start_' + number).innerText);
            if (insertCount > maxCount) {
                input.value = maxCount;
                insertCount = maxCount;
            }
            var remainingCount = insertCount + currentCount;
            document.getElementById('remaining_' + number).innerText = remainingCount;
        }
    </script>
</head>

<body>
    <?php
    session_start();
    require_once ('config.php');
    $host = DB_HOST;
    $user = DB_USER;
    $pass = DB_PASS;
    $db_name = DB_NAME;
    $db_table = DB_TABLE;
    $db_table_history = "sellAndLoadHistory";
    $link = mysqli_connect($host, $user, $pass, $db_name);

    if (!$link) {
        echo 'Не могу соединиться с БД. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
        exit;
    }
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        $count1 = $_POST['count1'];
        $count2 = $_POST['count2'];
        $count3 = $_POST['count3'];
        $count4 = $_POST['count4'];

        $count1 = mysqli_real_escape_string($link, $count1);
        $count2 = mysqli_real_escape_string($link, $count2);
        $count3 = mysqli_real_escape_string($link, $count3);
        $count4 = mysqli_real_escape_string($link, $count4);

        $date1 = $count1 == "0" ? 0 : $count1;
        $date2 = $count2 == "0" ? 0 : $count2;
        $date3 = $count3 == "0" ? 0 : $count3;
        $date4 = $count4 == "0" ? 0 : $count4;

        $update_query = "UPDATE $db_table SET `1Count` = `1Count` + '$count1', 
                        `2Count` = `2Count` + '$count2', 
                        `3Count` = `3Count` + '$count3', 
                        `4Count` = `4Count` + '$count4' 
                        WHERE `ID` = '{$_SESSION['username']}'";

        mysqli_query($link, $update_query);

        $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `1Count`, `2Count`, `3Count`, `4Count` FROM $db_table WHERE `ID` = '{$_SESSION['username']}'");
        $result = mysqli_fetch_assoc($sql);

        $sql = "INSERT INTO $db_table_history (`numberOfDevice`, `serial`, `address`, `cell1`, `cell2`, `cell3`, `cell4`, `isSell`, `remains1`, `remains2`, `remains3`, `remains4`) VALUES ({$_SESSION['username']}, '{$result['serial']}', '{$result['address']}', $date1, $date2, $date3, $date4, 0, {$result['1Count']}, {$result['2Count']}, {$result['3Count']}, {$result['4Count']})";
        if (!mysqli_query($link, $sql)) {
            echo "Error " . mysqli_error($link);
        }
        $command = "PR_{$result['1Count']}_{$result['2Count']}_{$result['3Count']}_{$result['4Count']}_";
        $sql = "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ({$_SESSION['username']},'$command')";
        if (mysqli_query($link, $sql)) {
            echo "Изменения успешно сохранены";
        } else {
            echo "Ошибка при сохранении данных: " . mysqli_error($link);
        }
        header("Location: {$_SERVER['PHP_SELF']}");
        exit();
    }
    ?>
    <div class="logout-container">
        <a href="logout.php" class="logout-btn">Выйти</a>
    </div>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <table border='1' id="maintable">
            <thead>
                <tr>
                    <th><b>Номер торгового аппарата</b></th>
                    <th><b>Серийный номер платежного терминала</b></th>
                    <th><b>Адрес установки аппарата</b></th>
                    <th><b>Товар</b></th>
                    <th><b>Количество товара, шт</b></th>
                    <th><b>Количество добавленного товара, шт</b></th>
                    <th><b>Остаток, шт</b></th>
                    <th><b>Внесенные изменения</b></th>
                </tr>
            </thead>
            <?php
            function formatChange($product, $date, $from, $to)
            {
                $added = $to - $from;
                $text = $from == null ? "Нет изменений" : "Ручной ввод: $date, изменение с $from шт на $to шт (добавлено $added шт товара)";
                return "Ячейка $product: $text";
            }

            $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '{$_SESSION['username']}' AND `isSell` = 0 AND `cell1` != 0 ORDER BY `ID` DESC LIMIT 1");
            $history_result = mysqli_fetch_assoc($history_query);
            $changes = [];
            $changes[] = formatChange(1, $history_result['datatime'], $history_result['remains1']-$history_result['cell1'], $history_result['remains1']);
            $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '{$_SESSION['username']}' AND `isSell` = 0 AND `cell2` != 0 ORDER BY `ID` DESC LIMIT 1");
            $history_result = mysqli_fetch_assoc($history_query);
            $changes[] = formatChange(2, $history_result['datatime'], $history_result['remains2']-$history_result['cell2'], $history_result['remains2']);
            $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '{$_SESSION['username']}' AND `isSell` = 0 AND `cell3` != 0 ORDER BY `ID` DESC LIMIT 1");
            $history_result = mysqli_fetch_assoc($history_query);
            $changes[] = formatChange(3, $history_result['datatime'], $history_result['remains3']-$history_result['cell3'], $history_result['remains3']);
            $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '{$_SESSION['username']}' AND `isSell` = 0 AND `cell4` != 0 ORDER BY `ID` DESC LIMIT 1");
            $history_result = mysqli_fetch_assoc($history_query);
            $changes[] = formatChange(4, $history_result['datatime'], $history_result['remains4']-$history_result['cell4'], $history_result['remains4']);
            $text = "";
            if (!empty($changes)) {
                $text = implode("<hr>", $changes);
            } else {
                $text = "Нет внесенных изменений.";
            }
            
            $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `1Count`, `2Count`, `3Count`, `4Count`, `cellLoad` FROM $db_table WHERE `ID` = '{$_SESSION['username']}'");
            $result = mysqli_fetch_array($sql);
            echo '<tr data-id="' . $result['ID'] . '">' .
                "<td rowspan='4'>{$result['ID']}</td>" .
                "<td rowspan='4'>{$result['serial']}</td>" .
                "<td rowspan='4'>{$result['address']}</td>" .
                "<td>№1</td>" .
                "<td id='start_1'>{$result['1Count']}</td>" .
                "<td><input type='number' name='count1' value='0' min='" . -$result['1Count'] . "' max='" . ($result['cellLoad'] - $result['1Count']) . "' onchange='updateRemaining(this, 1)'></td>" .
                "<td id='remaining_1'>{$result['1Count']}</td>" .
                "<td rowspan='4' id='changeCell'>$text</td>" .
                "</tr>" .
                "<tr>" .
                "<td>№2</td>" .
                "<td id='start_2'>{$result['2Count']}</td>" .
                "<td><input type='number' name='count2' value='0' min='" . -$result['2Count'] . "' max='" . ($result['cellLoad'] - $result['2Count']) . "' onchange='updateRemaining(this, 2)'></td>" .
                "<td id='remaining_2'>{$result['2Count']}</td>" .
                "</tr>" .
                "<tr>" .
                "<td>№3</td>" .
                "<td id='start_3'>{$result['3Count']}</td>" .
                "<td><input type='number' name='count3' value='0' min='" . -$result['3Count'] . "' max='" . ($result['cellLoad'] - $result['3Count']) . "' onchange='updateRemaining(this, 3)'></td>" .
                "<td id='remaining_3'>{$result['3Count']}</td>" .
                "</tr>" .
                "<tr>" .
                "<td>№4</td>" .
                "<td id='start_4'>{$result['4Count']}</td>" .
                "<td><input type='number' name='count4' value='0' min='" . -$result['4Count'] . "' max='" . ($result['cellLoad'] - $result['4Count']) . "' onchange='updateRemaining(this, 4)'></td>" .
                "<td id='remaining_4'>{$result['4Count']}</td>" .
                "</tr>";
            ?>
        </table>
        <div class="submit-container">
            <button type="submit" class="submit-btn">Обновить</button>
        </div>
    </form>
</body>

</html>