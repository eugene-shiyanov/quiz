project('quiz', 'c', default_options : ['c_std=c11'])
gtk = dependency('gtk+-3.0')
src = ['src/main.c', 'src/question.c', 'src/question_generator.c', 'src/ui.c']
executable('quiz', src, dependencies: [gtk])