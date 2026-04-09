# Créer un executable à partir de fichiers sources
add_executable(test_1  test_1.cxx)

# Fait le lien avec la bibliothèque créée précedemment
# Les entêtes sont automatiquement détectés.
target_link_libraries(test_1 particules)