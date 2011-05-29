<?php

class DeployWorkerQueue extends DeployWorkerDefault {

  function deploy(DeployEndpoint $endpoint) {
    foreach ($this->fetcher->getResources() as $resource) {
      // @todo Add resource and endpoint to queue.
    }
  }

}
