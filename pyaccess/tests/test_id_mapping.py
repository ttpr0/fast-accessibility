

from .. import _pyaccess_ext

def test_id_mapping():
    id_mapping = _pyaccess_ext.new_id_mapping(5)

    mapping = _pyaccess_ext.IntVector()
    mapping.append(2)
    mapping.append(4)
    mapping.append(1)
    mapping.append(0)
    mapping.append(3)

    id_mapping = _pyaccess_ext.reorder_targets(id_mapping, mapping)

    assert id_mapping.get_source(0) == 3
    assert id_mapping.get_source(1) == 2
    assert id_mapping.get_source(2) == 0
    assert id_mapping.get_source(3) == 4
    assert id_mapping.get_source(4) == 1

    assert id_mapping.get_target(0) == 2
    assert id_mapping.get_target(1) == 4
    assert id_mapping.get_target(2) == 1
    assert id_mapping.get_target(3) == 0
    assert id_mapping.get_target(4) == 3

    id_mapping = _pyaccess_ext.reorder_sources(id_mapping, mapping)

    assert id_mapping.get_source(0) == 0
    assert id_mapping.get_source(1) == 1
    assert id_mapping.get_source(2) == 2
    assert id_mapping.get_source(3) == 3
    assert id_mapping.get_source(4) == 4

    assert id_mapping.get_target(0) == 0
    assert id_mapping.get_target(1) == 1
    assert id_mapping.get_target(2) == 2
    assert id_mapping.get_target(3) == 3
    assert id_mapping.get_target(4) == 4
