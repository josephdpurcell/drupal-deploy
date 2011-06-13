<?php

class DeployEndpoint {

  var $authentication = NULL;

  var $service = NULL;

  function __construct(DeployAuthentication $authentication) {
    $this->authentication = $authentication;
  }

  function deploy(Traversable $iterator) {
    // TODO: Do something with authentication.

    // TODO: Find out where this piece of code really should go. It's not really
    // up to endpoints to figure out all this. It's probably most logical to let
    // the iterator do this. But in reallity iterators shouldn't do it either,
    // since processors might iterate them for the queueing process and then we
    // loose all the performance benifits of processors. And it's too much for
    // each service plugin to take care of. So left are the endpoint.
    $entity_wrappers = array();
    foreach ($iterator as $entity_id => $entity_type) {
      $entity = reset(entity_load($entity_type, array($entity_id)));
      $entity_wrappers[] = entity_metadata_wrapper($entity_type, $entity);
    }

    // This belongs here tough.
    $this->authentication->deploy($entity_wrappers);
  }

}
