# Instructions

## Docker setup
Pour télécharger Docker Desktop (pas besoin de créer de compte) / To download Docker Desktop (no need to create an account) : https://www.docker.com/products/docker-desktop/

## Compiling

Pour compiler votre code / To compile your code :

Linux : `docker run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc:latest make`

Windows : `docker run --rm -v ${PWD}:/usr/src/myapp -w /usr/src/myapp gcc:latest make`

## Packaging

Ensuite, faites un `.zip` avec le contenu de ce dossier (et non du dossier parent) / Then, create a `.zip` with the contents of this directory (and not the parent directory).
