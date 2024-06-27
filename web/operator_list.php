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
    $db_name = "users";
    $db_table = "users";
    $link = mysqli_connect($host, $user, $pass, $db_name);

    if (!$link) {
        echo 'Не могу соединиться с БД. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
        exit;
    }
    ?>
    <a href="admin_menu.php" class="back-link"><span>&#8592;</span> Меню</a>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <table border='1' id="maintable">
            <thead>
                <tr>
                    <th><b>Логин</b></th>
                    <th><b>Пароль</b></th>
                </tr>
            </thead>
            <?php
            $sql = mysqli_query($link, "SELECT `username`, `password` FROM $db_table WHERE `role` != 'admin'");
            while ($result = mysqli_fetch_array($sql)) {
                echo '<tr>' .
                    "<td>{$result['username']}</td>" .
                    "<td>{$result['password']}</td></tr>";
            }
            ?>
        </table>
    </form>
</body>


</html>