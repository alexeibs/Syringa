TEMPLATE      = subdirs

SUBDIRS = \
    syringa \
    tests

tests.depends = syringa
