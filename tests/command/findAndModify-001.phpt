--TEST--
MongoDB\Driver\Command with findAndModify and arrayFilters
--XFAIL--
START() tests must be reimplemented (PHPC-1179)
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php START('THROWAWAY', ["version" => "36-release"]); CLEANUP(THROWAWAY); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$manager = new MongoDB\Driver\Manager(THROWAWAY);

$bulk = new MongoDB\Driver\BulkWrite();

$bulk->insert([ '_id' => 1, 'grades' => [ 95, 92, 90 ] ]);
$bulk->insert([ '_id' => 2, 'grades' => [ 98, 100, 102 ] ]);
$bulk->insert([ '_id' => 3, 'grades' => [ 95, 110, 100 ] ]);

$manager->executeBulkWrite(DATABASE_NAME . '.' . COLLECTION_NAME, $bulk);

$command = new MongoDB\Driver\Command([
    'findAndModify' => COLLECTION_NAME, 
    'query' => ['grades' => [ '$gt' => 100 ] ], 
    'update' => ['$set' => [ 'grades.$[element]' => 100 ] ],
    'arrayFilters' => [ [ 'element' => [ '$gt' => 100 ] ] ],
]);

// Running this twice, because findAndModify only updates the first document
// it finds.
$manager->executeCommand(DATABASE_NAME, $command);
$manager->executeCommand(DATABASE_NAME, $command);

$cursor = $manager->executeQuery( DATABASE_NAME . '.' . COLLECTION_NAME, new \MongoDB\Driver\Query([]));
var_dump($cursor->toArray());
?>
===DONE===
<?php DELETE("THROWAWAY"); ?>
<?php exit(0); ?>
--CLEAN--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php DELETE("THROWAWAY"); ?>
--EXPECTF--
array(%d) {
  [0]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(1)
    ["grades"]=>
    array(%d) {
      [0]=>
      int(95)
      [1]=>
      int(92)
      [2]=>
      int(90)
    }
  }
  [1]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(2)
    ["grades"]=>
    array(%d) {
      [0]=>
      int(98)
      [1]=>
      int(100)
      [2]=>
      int(100)
    }
  }
  [2]=>
  object(stdClass)#%d (%d) {
    ["_id"]=>
    int(3)
    ["grades"]=>
    array(%d) {
      [0]=>
      int(95)
      [1]=>
      int(100)
      [2]=>
      int(100)
    }
  }
}
===DONE===
