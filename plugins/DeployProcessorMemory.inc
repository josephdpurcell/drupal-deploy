<?php

class DeployWorkerDefault implements DeployWorker {

  var $fetcher = NULL;

  function __construct(DeployFetcher $fetcher) {
    $this->fetcher = $fetcher;
  }

  function deploy(DeployEndpoint $endpoint) {
    $endpoint->render($this->fetcher->getResources());
  }

}
