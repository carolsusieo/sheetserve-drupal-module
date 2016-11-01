<?php
// sheetserve.inc

$sheetserve_node_id;
$sheetserve_key;
$sheetserve_title;

// MAKING CHANGES NOTE
// whenever you want to put in a new spreadsheet from a cvs, 
// add it, then update the sheetserve_node_id to the new node, and
// the service will start using the new spreadsheet
function _sheetserve_setup() {
	global $sheetserve_node_id;
	global $sheetserve_key;
	$sheetserve_key = "title";
	global $sheetserve_title;
	$sheetserve_title = "AnnieBank";
	$sheetserve_node_id = 88;
}

// sheetserve.inc
/**
* Callback for updating sheetnode resources.
*
* @param int $id
* @param object $data
* @return object
*/
function _sheetserve_update($id, $data) {  
  _sheetserve_setup();
  if (isset($data) &&  is_array($data)) 
  {

	if($id != 'update')
	{
	
		// need to get data from stuff sent in - but if update 
		// not sent in, we are running a test - need to set up data
		 $data = array(
			'amt' => '.01',
			'label' => 'no data',
			'currency' => 'USD',
			
		  );
	}
	else
	{
		watchdog('sheetserve','From inc Amt = "!amt" Label = "!label" 
		  Currency = " !currency',array('!amt' => $data['amt'],
		  '!label' =>$data['label'], 
		  '!currency' => $data['currency']),WATCHDOG_NOTICE);
   
	}	
	$sheet = sheetserve_put_transaction($data);

	$A2 = $sheet['cells']['A2']['datavalue'];
	$items = array('data' => $A2);
	
	// SAVE it
	sheetserve_write_sheetnode($sheet);
		
	return $items;
  }		
}

/**
* Callback for retrieving note resources.
*
* @param int $id
* @return object
*/
function _sheetserve_retrieve($id) {
	_sheetserve_setup();
	return _sheetserve_service();
}



/**
* Access callback for the  resource.
*
* @param string $op
*  The operation that's going to be performed.
* @param array $args
*  The arguments that will be passed to the callback.
* @return bool
*  Whether access is given or not.
*/

function _sheetserve_access($op, $args) {
  global $user;
  $access = FALSE;
  switch ($op) {
    case 'view':
      $sheetnode = sheetserve_get_sheetnode($args[0]);
      $access = user_access('sheetserve view any sheetnode');
      $access = $access || $sheetnode->uid == $user->uid && user_access('sheetserve view own sheetnodes');
      break;
    case 'update':
       $sheetnode = sheetserve_get_sheetnode($args[0]);
      $access = user_access('sheetserve edit any sheetnode');
      $access = $access || $sheetnode->uid == $user->uid && user_access('sheetserve edit own sheetnodes');
      break;
  }
 
  return $access;
}

function sheetserve_permission() {
  return array(
	'sheetserve view any sheetnode'=> array('title' => t('sheetserve view any sheetnode'),'description' => t('view any sheetnode'),),
	'sheetserve view own sheetnodes'=> array('title' => t('sheetserve view own sheetnodes'),'description' => t('view own sheetnodes'),),
	'sheetserve edit any sheetnode'=> array('title' => t('sheetserve edit any sheetnode'),'description' => t('edit any sheetnode'),),
	'sheetserve edit own sheetnodes'=> array('title' => t('sheetserve edit own sheetnodes'),'description' => t('edit own sheetnodes'),),
  );
}
?>
