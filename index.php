<!DOCTYPE html>
<html>
<head>
  <style>
    table {
      border-collapse: collapse;
      width: 50%;
      margin: auto;
    }

    th, td {
      text-align: left;
      padding: 8px;
    }
    tr:nth-child(even){background-color: #f2f2f2}
    th {
      background-color: #000;
      color: white;
    }
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>

</head>
<body>

<table>
  <thead>

  <tr>
    <th colspan="3" style="background:lightseagreen">All Records</th>
  </tr>
  <tr>
    <th>ID</th>
    <th>CARD ID</th>
    <th>DATE AND TIME</th>
  </tr>
  <thead>
  <tbody id="show">
  </tbody>
</table>

<script type="text/javascript">
  $(document).ready(function(){
    function showData()
    {
      $.ajax({
        url: 'process.php',
        type: 'POST',
        data: {action : 'showProcess'},
        dataType: 'html',
        success: function(result)
        {
          $('#show').html(result);
        },
        error: function()
        {
          alert("Failed to show the logs");
        }
      })
    }
    setInterval(function(){ showData(); }, 2500);
  });

</script>
</body>
</html>
