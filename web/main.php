<!doctype html>
<html lang="ru">

<head>
  <title>Основное</title>
  <style>
    #maintable {
      font-family: Arial, Helvetica, sans-serif;
      border-collapse: collapse;
      width: 100%;
      margin-top: 40px;
    }

    #maintable td,
    #maintable th {
      border: 1px solid #ddd;
      padding: 8px;
    }

    #maintable tr:nth-child(even) {
      background-color: #f2f2f2;
    }

    #maintable tr:hover {
      background-color: #ddd;
    }

    #maintable th {
      padding-top: 12px;
      padding-bottom: 12px;
      text-align: left;
      background-color: #04AA6D;
      color: white;
    }

    form {
      background-color: #fff;
      padding: 20px;
      border-radius: 5px;
      box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
      margin-bottom: 20px;
      font-family: 'Roboto', Arial, Helvetica, sans-serif;
    }

    form table {
      width: 100%;
    }

    form table td {
      padding: 8px 0;
    }

    form table td:first-child {
      width: 30%;
    }

    form input[type="text"],
    form input[type="number"],
    form input[type="datetime-local"] {
      width: 100%;
      padding: 8px;
      border: 1px solid #ccc;
      border-radius: 4px;
      box-sizing: border-box;
    }

    form input[type="submit"],
    form button {
      background-color: #04AA6D;
      color: white;
      padding: 10px 20px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
    }

    form input[type="submit"]:hover,
    form button:hover {
      background-color: #45a049;
    }

    .edit-button,
    .add-button {
      background-color: #04AA6D;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 5px;
      cursor: pointer;
    }

    .edit-button:hover,
    .add-button:hover {
      background-color: #057d4a;
    }

    .delete-button {
      background-color: #e74c3c;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 5px;
      cursor: pointer;
    }

    .delete-button:hover {
      background-color: #c0392b;
    }

    .button {
      background-color: #3498db;
      color: white;
      border: none;
      padding: 8px 12px;
      border-radius: 5px;
      cursor: pointer;
    }

    .button:hover {
      background-color: #2980b9;
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
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css">

  <script>
    function deleteRow(id) {
      if (confirm('Вы уверены, что хотите удалить эту строку?')) {
        window.location.href = '?delete_id=' + id;
      }
    }
    function reloadTerminal(id) {
      if (confirm('Вы уверены, что хотите перезагрузить терминал?')) {
        window.location.href = '?reloadTerminal=' + id;
      }
    }
    function reloadDevice(id) {
      if (confirm('Вы уверены, что хотите перезагрузить торговый аппарат?')) {
        window.location.href = '?reloadDevice=' + id;
      }
    }
    function burn(id) {
      if (confirm('Вы уверены, что хотите обновить прошивку?')) {
        window.location.href = '?burn=' + id;
      }
    }
  </script>
</head>

<body>
  <?php
  session_start();
  if ($_SESSION['role'] !== 'admin') {
    header("location: index.php");
    exit;
  }
  require_once 'config.php';
  $host = DB_HOST;
  $user = DB_USER;
  $pass = DB_PASS;
  $db_name = DB_NAME;
  $db_table = DB_TABLE;
  $link = mysqli_connect($host, $user, $pass, $db_name);
  function generatePassword($length = 8)
  {
    $chars = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
    $count = mb_strlen($chars);

    for ($i = 0, $result = ''; $i < $length; $i++) {
      $index = rand(0, $count - 1);
      $result .= mb_substr($chars, $index, 1);
    }

    return $result;
  }

  if (!$link) {
    echo 'Не могу соединиться с БД. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
    exit;
  }
  $sql = mysqli_query($link, "SELECT COUNT(*) as count FROM $db_table");
  $row = $sql->fetch_assoc();
  $row_count = $row["count"];
  if (isset($_GET['delete_id']) && is_numeric($_GET['delete_id']) && $_GET['delete_id'] >= 1) {
    $delete_id = $_GET['delete_id'];

    $sql_delete = "DELETE FROM {$db_table} WHERE `ID` = $delete_id";
    mysqli_query($link, $sql_delete);
    $link2 = mysqli_connect($host, $user, $pass, "users");
    if (!$link2) {
      echo 'Не могу соединиться с БД операторов. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
      exit;
    }
    $sql = mysqli_query($link2, "DELETE FROM `users` WHERE `username` = '$delete_id'");
    mysqli_close($link2);
    echo "<script>window.location.href = window.location.pathname;</script>";
    exit;
  }
  if (isset($_GET['reloadTerminal']) && is_numeric($_GET['reloadTerminal']) && $_GET['reloadTerminal'] >= 1) {
    $reloadTerminal = $_GET['reloadTerminal'];

    $sql = "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ($reloadTerminal, 'SW_OFF_T')";
    mysqli_query($link, $sql);
    echo "<script>window.location.href = window.location.pathname;</script>";
    exit;
  }
  if (isset($_GET['reloadDevice']) && is_numeric($_GET['reloadDevice']) && $_GET['reloadDevice'] >= 1) {
    $reloadTerminal = $_GET['reloadDevice'];

    $sql = "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ($reloadTerminal, 'REBOOT')";
    mysqli_query($link, $sql);
    echo "<script>window.location.href = window.location.pathname;</script>";
    exit;
  }
  if (isset($_GET['burn']) && is_numeric($_GET['burn']) && $_GET['burn'] >= 1) {
    $reloadTerminal = $_GET['burn'];

    $sql = "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ($reloadTerminal, 'BURN')";
    mysqli_query($link, $sql);
    echo "<script>window.location.href = window.location.pathname;</script>";
    exit;
  }
  if (isset($_POST["serial"], $_POST["address"] )) {
    if (isset($_GET['red_id'])) {
	    if (is_numeric($_GET['red_id'])) {
		    $address ="ADDR_". $_POST["address"];
		    $serial = "SN_".$_POST["serial"];
		    $sql = mysqli_query($link, "UPDATE $db_table SET `serial` = '{$_POST['serial']}', `address` = '{$_POST['address']}', `rentCost` = {$_POST['rentCost']}, `price` = {$_POST['price']}, `deviceCondition` = {$_POST['deviceCondition']}, `lastConnectionDate` = '{$_POST['lastConnectionDate']}', `lastSellDate` = '{$_POST['lastSellDate']}' WHERE `ID`={$_GET['red_id']}");
		    $sql = mysqli_query($link, "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ({$_GET['red_id']}, '$address')");
		    $sql = mysqli_query($link, "INSERT INTO `operations` (`numberOfDevice`, `command`) VALUES ({$_GET['red_id']}, '$serial')");
      } else {
        $sql = mysqli_query($link, "INSERT INTO $db_table (`ID`, `serial`, `address`, `rentCost`, `price`, `1Count`, `2Count`, `3Count`, `4Count`, `cellLoad`, `deviceCondition`, `lastConnectionDate`, `lastSellDate`) VALUES ({$_POST['ID']}, '{$_POST['serial']}', '{$_POST['address']}', {$_POST['rentCost']}, {$_POST['price']}, 0, 0, 0, 0, {$_POST['cellLoad']}, 1, NULL, NULL)");
        if (!$sql) {
          exit;
        }
        $link2 = mysqli_connect($host, $user, $pass, "users");
        if (!$link2) {
          echo 'Не могу соединиться с БД операторов. Код ошибки: ' . mysqli_connect_errno() . ', ошибка: ' . mysqli_connect_error();
          exit;
        }
        $pass = generatePassword();
        $sql = mysqli_query($link2, "INSERT INTO `users` (`username`, `password`, `role`) VALUES ({$_POST['ID']}, '$pass', 'user')");
        mysqli_close($link2);
      }
    }
    echo "<script>window.location.href = window.location.pathname;</script>";
  }

  ?>
  <a href="admin_menu.php" class="back-link"><span>&#8592;</span> Меню</a>
  <table border='1' id="maintable">
    <thead>
      <tr>
        <th><b>Номер торгового аппарата</b></th>
        <th><b>Серийный номер платежного терминала</b></th>
        <th><b>Адрес установки аппарата</b></th>
        <th><b>Стоимость аренды</b></th>
        <th><b>Наличие товара</b></th>
        <th><b>Состояние</b></th>
        <th><b>Последнее соединение с аппаратом</b></th>
        <th><b>Последняя продажа</b></th>
        <th><b>Действия</b></th>
      </tr>
    </thead>
    <?php
    $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `rentCost`, `1Count`, `2Count`, `3Count`, `4Count`, `deviceCondition`, `lastConnectionDate`, `isSuccessfulConnection`, `lastSellDate` FROM $db_table");
    while ($result = mysqli_fetch_array($sql)) {
      $productCount = $result['1Count'] + $result['2Count'] + $result['3Count'] + $result['4Count'];
      if ($result['1Count'] > 10 && $result['2Count'] > 10 && $result['3Count'] > 10 && $result['4Count'] > 10) {
        $productCountClass = 'green';
      } else {
        $productCountClass = 'red';
      }
      echo '<tr data-id="' . $result['ID'] . '">' .
        "<td>{$result['ID']}</td>" .
        "<td>{$result['serial']}</td>" .
        "<td>{$result['address']}</td>" .
        "<td>{$result['rentCost']} ₽</td>" .
        '<td>' .
        '<span style="color: ' . $productCountClass . '">' . $productCount . '</span>' .
        '<button title="Подробнее" class="icon-button" style="float:right;" onclick="location.href=\'details.php?product_id=' . $result['ID'] . '\'">' .
        '<i class="fas fa-info-circle"></i>' .
        '</button>' .
        '</td>' .
        "<td>" .
        '<div class="status-circle ' . ($result['deviceCondition'] == 0 ? 'red' : 'green') . '"></div>' .
        "</td>" .
        "<td style='color: " . ($result['isSuccessfulConnection'] == 0 ? 'red' : 'green') . "'>{$result['lastConnectionDate']}</td>" .
        "<td>{$result['lastSellDate']}</td>" .
        "<td>" .
        '<button title="Редактировать" class="edit-button" onclick="window.location.href=\'?red_id=' . $result['ID'] . '\'">' .
        '<i class="fas fa-edit"></i>' .
        '</button>' .
        '<button title="Удалить" class="delete-button" onclick="deleteRow(' . $result['ID'] . ')">' .
        '<i class="fas fa-trash"></i>' .
        '</button>' .
        '<button title="Телефоны" class="button" style="background-color: #7D44D6;" onclick="location.href=\'telephones.php?device=' . $result['ID'] . '\'">' .
        '<i class="fas fa-phone"></i>' .
        '</button>' .
        '<button title="Перезагрузить терминал" class="button" onclick="reloadTerminal(' . $result['ID'] . ')">' .
        '<i class="fas fa-spinner"></i>' .
        '</button>' .
        '<button title="Перезагрузить торговый аппарат" class="button" onclick="reloadDevice(' . $result['ID'] . ')">' .
        '<i class="fas fa-sun"></i>' .
        '</button>' .
        '<button title="Обновить прошивку" class="button" onclick="burn(' . $result['ID'] . ')">' .
        '<i class="fas fa-download"></i>' .
        '</button>' .
        "</td>" .
        '</tr>';
    }
    ?>
  </table>
  <div style="text-align: center; margin: 20px">
    <button title='Добавить новый аппарат' class='add-button' style="padding: 15px 30px;"
      onclick="window.location.href='?red_id=new'">
      <i class='fas fa-plus'> Добавить новый аппарат</i>
    </button>
  </div>
  <?php
  if (isset($_GET['red_id']) && (is_numeric($_GET['red_id']) && $_GET['red_id'] >= 1) || ($_GET['red_id'] == "new")) {
    if (is_numeric($_GET['red_id'])) {
      $sql = mysqli_query($link, "SELECT `ID`, `serial`, `address`, `rentCost`, `price`, `1Count`, `2Count`, `3Count`, `4Count`, `deviceCondition`, `lastConnectionDate`, `lastSellDate` FROM $db_table WHERE `ID`={$_GET['red_id']}");
      if (mysqli_num_rows($sql) == 0) {
        echo "<script>window.location.href = window.location.pathname;</script>";
      }
      $product = mysqli_fetch_array($sql);
    }
    ?>
    <form action="" method="post">
      <table>
        <thead>
          <tr>
            <?php
            if (is_numeric($_GET['red_id'])) {
              ?>
              <td colspan="2"><b>Редактировать данные аппарата №
                  <?= $product['ID'] ?>
                </b></td>
              <?php
            } else {
              ?>
              <td colspan="2"><b>Добавление нового аппарата</b></td>
              <?php
            }
            ?>
          </tr>
        </thead>
        <tbody>
          <?php
          if (!is_numeric($_GET['red_id'])) {
            ?>
            <tr>
              <td>Номер аппарата:</td>
              <td><input type="text" name="ID" value="<?= $product['ID'] ?>"></td>
            </tr>
            <?php
          }
          ?>
          <tr>
            <td>Серийный номер:</td>
            <td><input type="text" name="serial" value="<?= $product['serial'] ?>"></td>
          </tr>
          <tr>
            <td>Адрес установки:</td>
            <td><input type="text" name="address" value="<?= $product['address'] ?>"></td>
          </tr>
          <tr>
            <td>Стоимость аренды:</td>
            <td><input type="number" name="rentCost" value="<?= $product['rentCost'] ?>"></td>
          </tr>
          <tr>
            <td>Цена товара:</td>
            <td><input type="number" name="price" value="<?= $product['price'] ?>"></td>
          </tr>
          <?php
          if (!is_numeric($_GET['red_id'])) {
            ?>
            <tr>
              <td>Загрузка ячейки</td>
              <td><input type="number" name="cellLoad" value="<?= $product['cellLoad'] ?>"></td>
            </tr>
            <?php
          } else {
            ?>
            <tr>
              <td>Состояние:</td>
              <td>
                <select name="deviceCondition">
                  <option value="1" <?php if ($product['deviceCondition'] == 1)
                    echo 'selected="selected"'; ?>>Работает
                  </option>
                  <option value="0" <?php if ($product['deviceCondition'] == 0)
                    echo 'selected="selected"'; ?>>Ошибка</option>
                </select>
              </td>
            </tr>
            <tr>
              <td>Последнее соединение с аппаратом:</td>
              <td><input type="datetime-local" name="lastConnectionDate" value="<?= $product['lastConnectionDate'] ?>"></td>
            </tr>
            <tr>
              <td>Последняя продажа:</td>
              <td><input type="datetime-local" name="lastSellDate" value="<?= $product['lastSellDate'] ?>"></td>
            </tr>
            <?php
          }
          ?>
          <tr>
            <td colspan="2">
              <input type="submit" value="Сохранить">
              <button type="button"
                onclick="window.history.replaceState({}, document.title, window.location.pathname); window.location.reload()">Закрыть
                форму</button>
            </td>
          </tr>
        </tbody>
      </table>
    </form>
    <?php
  }
  mysqli_close($link);
  ?>
</body>

</html>
