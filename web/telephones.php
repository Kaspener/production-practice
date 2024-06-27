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
    $db_name = "vending";
    $db_table = "telephones";
    $link = mysqli_connect($host, $user, $pass, $db_name);

    if (!$link) {
        echo 'Не могу соединиться с БД. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
        exit;
    }

    if (isset($_POST['delete'])) {
        $phone_id = $_POST['delete'];
        $delete_query = "DELETE FROM $db_table WHERE `ID` = $phone_id";
        $select = "SELECT `telephone` FROM $db_table WHERE `numberOfDevice` = {$_POST['device']}";
        mysqli_query($link, $delete_query);
        $sql = mysqli_query($link, $select);
        $command = "TEL_";
        while ($result = mysqli_fetch_array($sql)) {
            $command .= $result['telephone'] . "_";
        }
        mysqli_query($link, $sql);
        mysqli_query($link, "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ({$_POST['device']}, '$command')");
        header("Location: telephones.php?device={$_POST['device']}");
        exit;
    }

    if (isset($_POST['new_phone'])) {
        $new_phone = $_POST['new_phone'];
        $device = $_POST['device'];
        $insert_query = "INSERT INTO $db_table (`numberOfDevice`, `telephone`) VALUES ($device, '$new_phone')";
        mysqli_query($link, $insert_query);
        $select = "SELECT `telephone` FROM $db_table WHERE `numberOfDevice` = {$_POST['device']}";
        $sql = mysqli_query($link, $select);
        $command = "TEL_";
        while ($result = mysqli_fetch_array($sql)) {
            $command .= $result['telephone'] . "_";
        }
        mysqli_query($link, $sql);
        mysqli_query($link, "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ({$_POST['device']}, '$command')");
        header("Location: telephones.php?device=$device");
        exit;
    }
    ?>
    <a href="admin_menu.php" class="back-link"><span>&#8592;</span> Меню</a><br><br>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <input type="hidden" name="device" value="<?php echo $_GET['device']; ?>">
        <label for="new_phone">Телефон:</label>
        <input id="new_phone" name="new_phone" type="tel" maxlength="50" autofocus="autofocus" required="required"
            value="+7__________" pattern="\+7\s?[\(]{0,1}9[0-9]{2}[\)]{0,1}\s?\d{3}[-]{0,1}\d{2}[-]{0,1}\d{2}"
            placeholder="+7__________">
        <button type="submit" id="addButton">Добавить</button>
    </form>
    <script type="text/javascript">
        function setCursorPosition(pos, e) {
            e.focus();
            if (e.setSelectionRange) e.setSelectionRange(pos, pos);
            else if (e.createTextRange) {
                var range = e.createTextRange();
                range.collapse(true);
                range.moveEnd("character", pos);
                range.moveStart("character", pos);
                range.select()
            }
        }

        function mask(e) {
            var matrix = this.placeholder,
                i = 0,
                def = matrix.replace(/\D/g, ""),
                val = this.value.replace(/\D/g, "");
            def.length >= val.length && (val = def);
            matrix = matrix.replace(/[_\d]/g, function (a) {
                return val.charAt(i++) || "_"
            });
            this.value = matrix;
            i = matrix.lastIndexOf(val.substr(-1));
            i < matrix.length && matrix != this.placeholder ? i++ : i = matrix.indexOf("_");
            setCursorPosition(i, this)
        }
        window.addEventListener("DOMContentLoaded", function () {
            var input = document.querySelector("#new_phone");
            input.addEventListener("input", mask, false);
            input.focus();
            setCursorPosition(3, input);
        });

        document.addEventListener('DOMContentLoaded', function () {
            var addButton = document.getElementById('addButton');
            var phoneInput = document.getElementById('new_phone');

            addButton.addEventListener('click', function (event) {
            if (phoneInput.value.includes('_')) {
                event.preventDefault(); 
                alert("Пожалуйста, заполните номер телефона полностью."); 
            }
        });
        });
    </script>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <table border='1' id="maintable">
            <thead>
                <tr>
                    <th><b>Телефон</b></th>
                    <th><b>Действия</b></th>
                </tr>
            </thead>
            <?php
            $sql = mysqli_query($link, "SELECT * FROM $db_table WHERE `numberOfDevice` = {$_GET['device']}");
            while ($result = mysqli_fetch_array($sql)) {
                echo '<tr>' .
                    "<td>{$result['telephone']}</td>" .
                    "<td>
                <form method='post' action='/telephones.php'>
                    <input type='hidden' name='device' value='{$_GET['device']}'>
                    <input type='hidden' name='delete' value='{$result['ID']}'>
                    <button type='submit'>Удалить</button>
                </form>
            </td>" .
                    '</tr>';
            }
            ?>
        </table>
    </form>
</body>

</html>