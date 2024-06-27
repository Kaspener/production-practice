<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Детали торгового аппарата</title>
    <style>
        #detailstable {
            font-family: Arial, Helvetica, sans-serif;
            border-collapse: collapse;
            width: 100%;
        }

        #detailstable td,
        #detailstable th {
            border: 1px solid #ddd;
            padding: 8px;
        }

        #detailstable tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        #detailstable th {
            padding-top: 12px;
            padding-bottom: 12px;
            text-align: left;
            background-color: #04AA6D;
            color: white;
        }

        .low-stock {
            color: red;
        }

        .page-title {
            font-size: 24px;
            color: #04AA6D;
            text-align: center;
            margin-bottom: 20px;
            text-transform: uppercase;
            letter-spacing: 2px;
        }

        .edit-mode {
            background-color: #f0f0f0;
        }

        .editable-cell {
            cursor: pointer;
        }

        .editable-cell.edit-mode {
            cursor: auto;
        }

        .editable-cell.edit-mode input[type="number"] {
            width: 60px;
        }

        .editable-cell input[type="number"] {
            width: 60px;
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 3px;
            font-size: inherit;
            box-sizing: border-box;
        }

        .editable-cell input[type="number"]:focus {
            outline: none;
            border-color: #66afe9;
            box-shadow: 0 0 5px #66afe9;
        }

        .change-block {
            margin-bottom: 5px;
        }

        .change-block span {
            font-weight: bold;
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

        button[type="submit"] {
            background-color: #04AA6D;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
            transition: background-color 0.3s ease;
            margin-top: 20px;
        }

        button[type="submit"]:hover {
            background-color: #21825c;
        }

        .form-container {
            display: flex;
            justify-content: center;
            margin-top: 20px;
        }
    </style>
</head>

<body>
    <?php
    session_start();
    if ($_SESSION['role'] !== 'admin') {
        header("location: index.php");
        exit;
    }
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
        if (isset($_POST['1Count'], $_POST['2Count'], $_POST['3Count'], $_POST['4Count'], $_POST['ID'])) {
            $oneCount = $_POST['1Count'];
            $twoCount = $_POST['2Count'];
            $threeCount = $_POST['3Count'];
            $fourCount = $_POST['4Count'];
            $id = $_POST['ID'];

            $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `1Count`, `2Count`, `3Count`, `4Count`, `cellLoad` FROM $db_table WHERE `ID` = '$id'");
            $result = mysqli_fetch_assoc($sql);

            $sql = "UPDATE " . DB_TABLE . " SET `1Count` = '$oneCount', `2Count` = '$twoCount', `3Count` = '$threeCount', `4Count` = '$fourCount' WHERE `ID` = '$id'";

            if (mysqli_query($link, $sql)) {
                echo "Изменения успешно сохранены";
            } else {
                echo "Ошибка при сохранении данных: " . mysqli_error($link);
            }
            $date1 = $result['1Count'] == $oneCount ? 0 : $oneCount - $result['1Count'];
            $date2 = $result['2Count'] == $twoCount ? 0 : $twoCount - $result['2Count'];
            $date3 = $result['3Count'] == $threeCount ? 0 : $threeCount - $result['3Count'];
            $date4 = $result['4Count'] == $fourCount ? 0 : $fourCount - $result['4Count'];

            $sql = "INSERT INTO $db_table_history (`numberOfDevice`, `serial`, `address`, `cell1`, `cell2`, `cell3`, `cell4`, `isSell`, `remains1`, `remains2`, `remains3`, `remains4`) VALUES ($id, '{$result['serial']}', '{$result['address']}', $date1, $date2, $date3, $date4, 0, $oneCount, $twoCount, $threeCount, $fourCount)";
            if (mysqli_query($link, $sql)) {
                echo "Изменения успешно сохранены";
            } else {
                echo "Ошибка при сохранении данных: " . mysqli_error($link);
            }
            $command = "PR_{$oneCount}_{$twoCount}_{$threeCount}_{$fourCount}_";
            $sql = "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ($id,'$command')";
            if (mysqli_query($link, $sql)) {
                echo "Изменения успешно сохранены";
            } else {
                echo "Ошибка при сохранении данных: " . mysqli_error($link);
            }
            mysqli_close($link);
            header('Location: ' . $_SERVER['HTTP_REFERER']);
            exit;
        } else {
            echo "Не все необходимые данные были отправлены";
        }
    }
    if (isset($_GET['product_id'])) {
        $product_id = $_GET['product_id'];

        $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `1Count`, `2Count`, `3Count`, `4Count`, `cellLoad` FROM $db_table WHERE `ID` = '$product_id'");
        $result = mysqli_fetch_assoc($sql);

        if (!$result) {
            echo "Торговый аппарат с указанным ID не найден.";
            exit;
        }
        $totalCount = $result["cellLoad"] * 4;
    } else {
        echo "Не указан ID торгового аппарата.";
        exit;
    }
    ?>
    <a href="main.php" class="back-link"><span>&#8592;</span> Вернуться</a>
    <h1 class="page-title">Детали торгового аппарата №
        <?php echo $result['ID']; ?>
    </h1>
    <form id="saveForm" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
        <input type="hidden" name="ID" value="<?php echo $result['ID']; ?>">
        <input type="hidden" id="1CountHidden" name="1Count">
        <input type="hidden" id="2CountHidden" name="2Count">
        <input type="hidden" id="3CountHidden" name="3Count">
        <input type="hidden" id="4CountHidden" name="4Count">
        <table border='1' id="detailstable">
            <thead>
                <tr>
                    <th>Номер торгового аппарата</th>
                    <th>Серийный номер платежного терминала</th>
                    <th>Адрес установки аппарата</th>
                    <th>Максимальная загрузка (всего/по каждой ячейке)</th>
                    <th>Количество товаров в ячейке 1</th>
                    <th>Количество товаров в ячейке 2</th>
                    <th>Количество товаров в ячейке 3</th>
                    <th>Количество товаров в ячейке 4</th>
                    <th>Внесенные изменения</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <?php echo $result['ID']; ?>
                    </td>
                    <td>
                        <?php echo $result['serial']; ?>
                    </td>
                    <td>
                        <?php echo $result['address']; ?>
                    </td>
                    <td>
                        <?php echo "$totalCount/{$result['cellLoad']}"; ?>
                    </td>
                    <td class="editable-cell <?php echo ($result['1Count'] <= 10) ? 'low-stock' : ''; ?>" id="1Count"
                        ondblclick="enableEditMode(this, 1)">
                        <span name="1Count">
                            <?php echo $result['1Count']; ?>
                        </span>
                    </td>
                    <td class="editable-cell <?php echo ($result['2Count'] <= 10) ? 'low-stock' : ''; ?>" id="2Count"
                        ondblclick="enableEditMode(this, 2)">
                        <span name="2Count">
                            <?php echo $result['2Count']; ?>
                        </span>
                    </td>
                    <td class="editable-cell <?php echo ($result['3Count'] <= 10) ? 'low-stock' : ''; ?>" id="3Count"
                        ondblclick="enableEditMode(this, 3)">
                        <span name="3Count">
                            <?php echo $result['3Count']; ?>
                        </span>
                    </td>
                    <td class="editable-cell <?php echo ($result['4Count'] <= 10) ? 'low-stock' : ''; ?>" id="4Count"
                        ondblclick="enableEditMode(this, 4)">
                        <span name="4Count">
                            <?php echo $result['4Count']; ?>
                        </span>
                    </td>
                    <?php
                    function formatChange($product, $date, $from, $to)
                    {
                        $added = $to - $from;
                        $text = $from == null ? "Нет изменений" : "Ручной ввод: $date, изменение с $from шт на $to шт (добавлено $added шт товара)";
                        return "Ячейка $product: $text";
                    }

                    $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '$product_id' AND `isSell` = 0 AND `cell1` != 0 ORDER BY `ID` DESC LIMIT 1");
                    $history_result = mysqli_fetch_assoc($history_query);
                    $changes = [];
                    $changes[] = formatChange(1, $history_result['datatime'], $history_result['remains1']-$history_result['cell1'], $history_result['remains1']);
                    $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '$product_id' AND `isSell` = 0 AND `cell2` != 0 ORDER BY `ID` DESC LIMIT 1");
                    $history_result = mysqli_fetch_assoc($history_query);
                    $changes[] = formatChange(2, $history_result['datatime'], $history_result['remains2']-$history_result['cell2'], $history_result['remains2']);
                    $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '$product_id' AND `isSell` = 0 AND `cell3` != 0 ORDER BY `ID` DESC LIMIT 1");
                    $history_result = mysqli_fetch_assoc($history_query);
                    $changes[] = formatChange(3, $history_result['datatime'], $history_result['remains3']-$history_result['cell3'], $history_result['remains3']);
                    $history_query = mysqli_query($link, "SELECT * FROM $db_table_history WHERE `numberOfDevice` = '$product_id' AND `isSell` = 0 AND `cell4` != 0 ORDER BY `ID` DESC LIMIT 1");
                    $history_result = mysqli_fetch_assoc($history_query);
                    $changes[] = formatChange(4, $history_result['datatime'], $history_result['remains4']-$history_result['cell4'], $history_result['remains4']);
                    ?>

                    <td id="changeCell">
                        <?php
                        if (!empty($changes)) {
                            echo implode("<hr>", $changes);
                        } else {
                            echo "Нет внесенных изменений.";
                        }
                        ?>
                    </td>

                </tr>
            </tbody>
        </table>
        <div class="form-container">
            <button type="submit">Сохранить изменения</button>
        </div>
    </form>

    <script>
        function enableEditMode(element, cellNumber) {
            var currentValue = element.innerText;
            element.classList.add('edit-mode');
            var inputElement = document.createElement('input');
            inputElement.type = 'number';
            inputElement.value = currentValue;
            inputElement.min = '0';
            inputElement.max = '<?php echo $result['cellLoad']; ?>';
            inputElement.addEventListener('blur', function () {
                disableEditMode(element, inputElement, currentValue, cellNumber);
            });
            element.innerText = '';
            element.appendChild(inputElement);
            inputElement.focus();
        }

        function disableEditMode(element, inputElement, currentValue, cellNumber) {
            var newValue = parseInt(inputElement.value);
            if (isNaN(newValue) || newValue < 0) {
                newValue = 0;
            } else if (newValue > parseInt(inputElement.max)) {
                newValue = parseInt(inputElement.max);
            }
            element.classList.remove('edit-mode');
            element.innerText = newValue;

            if (newValue !== parseInt(currentValue)) {
                if (newValue <= 10) {
                    element.classList.add('low-stock');
                } else {
                    element.classList.remove('low-stock');
                }

                // var currentDate = new Date();
                // var dateFormatted = currentDate.toLocaleDateString();
                // var timeFormatted = currentDate.toLocaleTimeString();

                // var changeText = "Ячейка " + cellNumber + ": ручной ввод: " + dateFormatted + ", " + timeFormatted + ", изменение с " + currentValue + " шт на " + newValue + " шт (добавлено " + (newValue - parseInt(currentValue)) + " шт товара)";

                // var changeCell = document.getElementById('changeCell');
                // if (changeCell) {
                //     var newChange = document.createElement('span');
                //     newChange.innerText = changeText;
                //     var divider = document.createElement('hr');
                //     changeCell.appendChild(newChange);
                //     changeCell.appendChild(divider);
                // } else {
                //     console.error("Элемент с id 'changeCell' не найден.");
                // }
            }
        }
        function updateHiddenFields() {
            document.getElementById('1CountHidden').value = document.getElementById('1Count').innerText;
            document.getElementById('2CountHidden').value = document.getElementById('2Count').innerText;
            document.getElementById('3CountHidden').value = document.getElementById('3Count').innerText;
            document.getElementById('4CountHidden').value = document.getElementById('4Count').innerText;
        }

        document.getElementById('saveForm').addEventListener('submit', function () {
            updateHiddenFields();
        });
    </script>
</body>

</html>