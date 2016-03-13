#!/bin/bash

user="cgs"
dir="/home/$user/code/mkvid"

function update {
  dir="${1}"
  if [[ -z "${dir}" ]]
  then
    echo "dir required"
    exit 1
  fi

  echo "change pwd to \"${dir}\""
  cd "${dir}"
  echo "git pull from origin"
  git pull origin master
  echo "touch uwsgi/reload"
  touch uwsgi/reload
}

export -f update

echo "su to \"${user}\""
su "${user}" -c "update \"${dir}\""

echo "Pushed!"
