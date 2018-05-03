--TEST--
MongoDB\Driver\Monitoring\CommandFailedEvent: requestId and operationId match
--SKIPIF--
<?php require __DIR__ . "/../utils/basic-skipif.inc"; ?>
<?php skip_if_not_live(); ?>
<?php skip_if_not_clean(); ?>
--FILE--
<?php
require_once __DIR__ . "/../utils/basic.inc";

$m = new MongoDB\Driver\Manager(URI);

class MySubscriber implements MongoDB\Driver\Monitoring\CommandSubscriber
{
    public function commandStarted( \MongoDB\Driver\Monitoring\CommandStartedEvent $event )
    {
        echo "started: ", $event->getCommandName(), "\n";
        $this->startRequestId = $event->getRequestId();
        $this->startOperationId = $event->getOperationId();
    }

    public function commandSucceeded( \MongoDB\Driver\Monitoring\CommandSucceededEvent $event )
    {
    }

    public function commandFailed( \MongoDB\Driver\Monitoring\CommandFailedEvent $event )
    {
        echo "failed: ", $event->getCommandName(), "\n";
        echo "- requestId matches: ", $this->startRequestId == $event->getRequestId() ? 'yes' : 'no', " \n";
        echo "- operationId matches: ", $this->startOperationId == $event->getOperationId() ? 'yes' : 'no', " \n";
    }
}

$query = new MongoDB\Driver\Query( [] );
$subscriber = new MySubscriber;

MongoDB\Driver\Monitoring\addSubscriber( $subscriber );

drop_collection(URI, DATABASE_NAME, COLLECTION_NAME);
?>
--EXPECT--
started: drop
failed: drop
- requestId matches: yes 
- operationId matches: yes
