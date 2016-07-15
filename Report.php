<?php
$rpt_file = $_REQUEST['rpt_file'];
require_once("Java.inc");

java_require("./");
//$rpt = new java("com.jeki.Report","Reports/".$rpt_file);

$rpt = new java("com.jeki.Report","Reports/".$rpt_file,true);
/*$rpt->setDataRow(
	array(
		array("kelamin","Pria"),
		array("agama","Protestan"),
		array("alamat","Sidoarjo"),
		array("ijazah_terakhir1","SMP")
	)
	
);

/**/

if(isset($_POST['params'])){
	$params = json_decode($_POST['params'],true);
	foreach($params as $key=>$param){
		$rpt->setParams($key,$param);
	}
}

if(isset($_POST['fields'])){
	$fields = json_decode($_POST['fields'],true);
	foreach($fields as $d){
		$rpt->setDataRow($d);
	}
}

if(isset($_POST['struk'])){
	if(get_magic_quotes_gpc()){
	  $struk = stripslashes($_POST['struk']);
	}else{
	  $struk = $_POST['struk'];
	}
	$struk = json_decode($struk);
	$rpt->setDataRow($struk);
}
$rpt->report();

die();
?>
