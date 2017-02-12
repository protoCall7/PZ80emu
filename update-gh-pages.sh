if [ "$TRAVIS_PULL_REQUEST" == "false" ]; then
  echo -e "Starting to update gh-pages\n"

  #copy data we're interested in to other place
  cp -R PZ80emu-0.1-coverage $HOME/coverage
  cp -R doxygen-doc/html $HOME/docs
  cp tests/test-suite.log $HOME/test-suite.log

  #go to home and setup git
  cd $HOME
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis"

  #using token clone gh-pages branch
  git clone --quiet --branch=gh-pages https://${GH_TOKEN}@github.com/protoCall7/PZ80emu.git gh-pages > /dev/null

  #go into diractory and copy data we're interested in to that directory
  cd gh-pages
  git rm -rf coverage
  cp -Rf $HOME/coverage .
  cp -Rf $HOME/docs .
  cp $HOME/test-suite.log .

  #clean, add, commit and push files
  git add -f .
  git commit -m "Travis build $TRAVIS_BUILD_NUMBER pushed to gh-pages"
  git push -fq origin gh-pages > /dev/null

  echo -e "Done magic with coverage\n"
fi
