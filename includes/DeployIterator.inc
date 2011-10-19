<?php

/**
 * Just a wrapper for now.
 */
class DeployIteratorIterator extends RecursiveIteratorIterator {}

/**
 * Iterator which does the heavy lifting for detecting dependencies.
 *
 * This iterator is reponsible for taking in an array of entity types and ids,
 * figure out all their dependencies and return an iterable object of all of
 * them, in a deployable order (dependencies first). And since dependencies
 * in theory are nested and recursive, we are using a recursive iterator here.
 *
 * @todo
 *   We need to throw an exception when we detect a circular dependency.
 *
 * @todo
 *   Look into Entity Property API to see if we can do anything cool/effective
 *   with the chaining feature there.
 */
class DeployIterator implements RecursiveIterator {

  var $entities = array();

  var $dependencies = array();

  var $belongings = array();

  var $entityType = NULL;

  var $traversed = array();

  /**
   * Constructor.
   *
   * The $entities array should be structured as below, where 'foo', 'bar' and
   * 'baz' are entity types and the numeric keys are entity ids. Although it's
   * not required, the value of each key can be a similar structured array of
   * dependencies that will be parsed recursively.
   * @code
   *   $entities = array(
   *     'node' => array(
   *       10 => TRUE,
   *       12 => array(
   *         'taxonomy_term' => array(
   *           14 => TRUE,
   *         ),
   *         'user' => array(
   *           8 => TRUE,
   *         ),
   *       ),
   *     ),
   *     'taxonomy_term' => array(
   *       16 => TRUE,
   *     ),
   *   );
   * @endcode
   *
   * @param $entities
   *   A structured array of entity ids and their entity types.
   */
  function __construct($entities, $traversed = array(), $parent = NULL) {
    $this->entities = $entities;
    $this->traversed = $traversed;
    $this->parent = $parent;
  }

  function hasChildren() {
    $key = $this->key();
    $current = $this->current();
    if (is_array($current) && !isset($current['entity'])) {
      $this->entityType = $key;
      return TRUE;
    }

    // Don't check for dependencies twice. They should already have been taken
    // into account for it it was traversed before.
    if (!isset($this->traversed[$this->entityType][$key])) {
      if (!empty($current['entity'])) {
        // Since the current entity is a UUID entity (with all properties and
        // field references as UUIDs) we need to re-load it as a standard entity
        // to make life easier for implementators of the following hooks. This
        // should have minor performance implications since the entity should
        // be cached at this point.
        $entity = reset(entity_load($this->entityType, array($current['entity_id'])));

        // Fetch dependencies and belongings.
        $this->dependencies[$key] = module_invoke_all('deploy_entity_dependencies', $entity, $this->entityType);
        $this->belongings[$key] = module_invoke_all('deploy_entity_belongings', $entity, $this->entityType);

        // Don't add dependencies that was already traversed.
        foreach ($this->dependencies[$key] as $entity_type => $dependencies) {
          foreach ($dependencies as $entity_id => $something) {
            if (isset($this->traversed[$entity_type][$entity_id])) {
              unset($this->dependencies[$key][$entity_type][$entity_id]);
            }
          }
        }

        // Let other modules have their say.
        drupal_alter('deploy_entity_dependencies', $this->dependencies[$key], $entity, $this->entityType);
        drupal_alter('deploy_entity_belongings', $this->belongings[$key], $entity, $this->entityType);
      }

      // Now mark this current entity as traversed.
      $this->traversed[$this->entityType][$key] = TRUE;
      if (!empty($this->parent)) {
        $this->parent->traversed[$this->entityType][$key] = TRUE;
      }

      if (!empty($this->dependencies[$key])) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Gets the children.
   */
  function getChildren() {
    $key = $this->key();
    $current = $this->current();

    $entities = array();
    // Check if we have dependencies directly in the tree in the form of a
    // nested array.
    if (is_array($current) && !isset($current['entity'])) {
      $entities = $current;
    }

    if (!empty($this->dependencies[$key]) || !empty($this->belongings[$key])) {
      // TODO: We need to merge with $entities from above in some smart way.
      $entities = $this->dependencies[$key];
      // In an iterator, having children means that the current key it self
      // isn't a part of the entities. However, this isn't the case when doing
      // deployments. So we add the parent as a part of the entities.
      // And since children always should go first, we add the parent last.
      $entities[$this->entityType][$key] = TRUE;

      // TODO: Merge in $this->belongings[$key] to entities.

    }

    $iterator = new DeployIterator($entities, $this->traversed, $this);
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  /**
   * Get the current value (entity).
   *
   * @todo Implement static caching to avoid multiple entity loads.
   */
  function current() {
    $current = current($this->entities);
    if (is_array($current)) {
      return $current;
    }
    // Sort out some keys.
    $key = $this->key();
    $uuid = reset(entity_get_uuid_by_id($this->entityType, array($key)));
    // Load the current entity with UUID.
    $entity = reset(entity_uuid_load($this->entityType, array($uuid)));

    // Let other modules have their say.
    drupal_alter('deploy_entity', $entity, $this->entityType);

    return array(
      'entity_type' => $this->entityType,
      'entity_id' => $key,
      'entity' => $entity,
    );
  }

  function key() {
    return key($this->entities);
  }

  function next() {
    return next($this->entities);
  }

  function rewind() {
    reset($this->entities);
  }

  function valid() {
    $key = key($this->entities);
    return ($key !== NULL && $key !== FALSE);
  }
}
