<?php
/**
 * @file Sheetserve callback routines used by services.
 *
 * Respond to external applications.
 */

/**
 * Callback for updating sheetnode resources.
 *
 * @param int $id
 *   If 'update' will extract data sent in for spreadsheet.
 * @param object $data
 *   Struct containing the amt, label and currency for transaction.
 *
 * @return object Contains current amount for the bank account,
 *         after this transaction.
 */
function _sheetserve_update($id, $data) {

  if (isset($data) &&  is_array($data)) {
    if ($id != 'update') {
      // Need to get data from stuff sent in.
      $data = array(
        'amt' => '.01',
        'label' => 'no data',
        'currency' => 'USD',

      );
    }
    else {
      // $data = $data['data'];.
      watchdog('sheetserve',
      'From inc Amt = "!amt" Label = "!label" Currency = " !currency',
      array(
        '!amt' => $data['amt'],
        '!label' => $data['label'],
        '!currency' => $data['currency'],
      ), WATCHDOG_NOTICE);

    }
    $sheet = sheetserve_put_transaction($data);

    $balance = $sheet['cells']['A2']['datavalue'];
    $items = array('data' => $balance);

    // SAVE it.
    sheetserve_write_sheetnode($sheet);

    return $items;
  }
}

/**
 * Callback for retrieving note resources.
 *
 * @param int $id
 *   Not used at this time.
 *
 * @return string
 *   Balance of account.
 */
function _sheetserve_retrieve($id) {
  return _sheetserve_service();
}

/**
 * Access callback for the note resource.
 *
 * @param string $op
 *   The operation that's going to be performed: 'view' or 'update'.
 * @param array $args
 *   The arguments that will be passed to the callback.
 *
 * @return bool
 *   Whether access is given or not.
 */
function _sheetserve_access($op, $args) {
  global $user;
  $access = FALSE;
  switch ($op) {
    case 'view':
      $sheetnode = sheetserve_get_sheetnode($args[0]);
      $access = user_access('sheetserve view any sheetnode');
      $access = $access || $sheetnode->uid == $user->uid
      && user_access('sheetserve view own sheetnodes');
      break;

    case 'update':
      /*Currently id is the word id, because it is not used...
       *there is only one sheetnode
       *it thinks there is no arguments sent in....
       */
      $sheetnode = sheetserve_get_sheetnode($args[0]);
      $access = user_access('sheetserve edit any sheetnode');
      $access = $access || $sheetnode->uid == $user->uid
      && user_access('sheetserve edit own sheetnodes');
      break;
  }
  // Seems to me access would exit for everything...
  $access = TRUE;

  return $access;
}

/**
 * Permission callback for node resource.
 *
 * @return array Permissions for display in drupal.
 *    Whether access is given or not.
 */
function sheetserve_permission() {
  return array(
    'sheetserve view any sheetnode' =>
        array(
          'title' => t('sheetserve view any sheetnode'),
          'description' => t('view any sheetnode'),
        ),
    'sheetserve view own sheetnodes' =>
        array(
          'title' => t('sheetserve view own sheetnodes'),
          'description' => t('view own sheetnodes'),
        ),
    'sheetserve edit any sheetnode' =>
        array(
          'title' => t('sheetserve edit any sheetnode'),
          'description' => t('edit any sheetnode'),
        ),
    'sheetserve edit own sheetnodes' =>
        array(
          'title' => t('sheetserve edit own sheetnodes'),
          'description' => t('edit own sheetnodes'),
        ),
  );
}
