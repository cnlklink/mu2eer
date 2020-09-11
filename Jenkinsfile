pipeline {

// agent any

  agent {
     node { 
        label 'ad130482'
        customWorkspace "build/achilles_generic_buildroot"
        } 
  }

  stages {

    stage('Make') {
      steps {
        sh 'cd src; make'
      }
    }

    stage('Make Tests') {
      steps {
        sh 'cd src; make tests'
      }
    }

    stage('Cucumber tests') {
      steps {
        sh '/home/jdiamond/bin/cucumber'
      }
    }

  }
  triggers {
    pollSCM('H/5 * * * *')
  }
}
