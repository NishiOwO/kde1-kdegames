/*
 *  ksokoban - a Sokoban game for KDE
 *  Copyright (C) 1998  Anders Widell  <d95-awi@nada.kth.se>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <kapp.h>
#include <ktopwidget.h>
#include <kmenubar.h>
#include <qpopmenu.h>
#include <qkeycode.h>
#include <kstatusbar.h>
#include <qlabel.h>

#include "MainWindow.H"
#include "PlayField.H"

#include "MainWindow.moc.C"

MainWindow::MainWindow() : KTopLevelWidget()
{
  playField_ = new PlayField (this, "pf");
  setView (playField_);

  menu_ = new KMenuBar(this, "_mainMenu" );
  game_ = new QPopupMenu(0,"fgame");
  game_->insertItem ("Next level", playField_, SLOT(nextLevel()), Key_N);
  game_->insertItem ("Previous level", playField_, SLOT(previousLevel()), Key_P);
  game_->insertItem ("Restart level", playField_, SLOT(restartLevel()), Key_Escape);

  set_ = new QPopupMenu(0,"fset");
  set_->setCheckable (true);
  connect (set_, SIGNAL(activated(int)), playField_, SLOT(changeSet(int)));
  connect (set_, SIGNAL(activated(int)), this, SLOT(updateSetMenu(int)));
  for (int i=0; i<playField_->noOfSets (); i++) {
    set_->insertItem (playField_->setName (i), i);
  }
  checkedSet_=0;
  updateSetMenu (playField_->set ());
  game_->insertItem ("Level collection", set_);

  game_->insertItem ("Undo", playField_, SLOT(undo()), Key_U);
  game_->insertItem ("Redo", playField_, SLOT(redo()), Key_R);
  game_->insertSeparator ();
  game_->insertItem ("Quit", KApplication::getKApplication (), SLOT(quit()), Key_Q);
  menu_->insertItem("Game", game_);

  graphics_ = new QPopupMenu(0,"fgraphics");
  graphics_->setCheckable (true);
  connect (graphics_, SIGNAL(activated(int)), this, SLOT(changeGfx(int)));
  graphics_->insertItem ("Small", 0);
  graphics_->insertItem ("Medium", 1);
  graphics_->insertItem ("Large", 2);
  checkedGfx_=playField_->resolution ();
  updateGfxMenu (checkedGfx_);
  menu_->insertItem("Graphics", graphics_);

  animation_ = new QPopupMenu(0,"fanimation");
  animation_->setCheckable (true);
  connect (animation_, SIGNAL(activated(int)), this, SLOT(updateAnimMenu(int)));
  connect (animation_, SIGNAL(activated(int)), playField_, SLOT(changeAnim(int)));
  animation_->insertItem ("Slow", 3);
  animation_->insertItem ("Medium", 2);
  animation_->insertItem ("Fast", 1);
  animation_->insertItem ("Off", 0);
  checkedAnim_ = playField_->animDelay ();
  updateAnimMenu (checkedAnim_);
  menu_->insertItem("Animation", animation_);

  KApplication *app = KApplication::getKApplication ();
  help_ = app->getHelpMenu (true, "\
ksokoban 0.1.2 - a Sokoban game for KDE

ksokoban is copyright (c) 1998 Anders Widell <d95-awi@nada.kth.se>
See http://hem.passagen.se/awl/ksokoban/ for more info on ksokoban

The levels are copyrighted by their authors:

Original - the first 50 original levels from xsokoban
Extra - some additional levels from xsokoban
Still more - by J. Franklin Mentzer <wryter@aol.com>
MacTommy inventions - by Tommy in Pennsylvania
Yoshio's autogenerated - by Yoshio Murase <yoshio@asahi.email.ne.jp>
For the kids - by Ross (W.R.) Brown <ross@bnr.ca>
Dimitri & Yorick - by Jacques Duthen <duthen@club-internet.fr>
Simple Sokoban - by Phil Shapiro <pshapiro@his.com>
");
  menu_->insertSeparator ();
  menu_->insertItem("Help", help_);


  setMenu(menu_);
  menu_->show();

  statusBar_ = new KStatusBar (this, "statusBar");
  statusBar_->setInsertOrder (KStatusBar::RightToLeft);
  statusBar_->setBorderWidth (3);
  //collectionLabel_ = new QLabel ("None           ", statusBar_);
  //statusBar_->insertWidget (collectionLabel_, 20, 2);
  //levelLabel_ = new QLabel ("   3", statusBar_);
  //statusBar_->insertWidget (levelLabel_, 20, 1);
  statusBar_->insertItem ("Pushes: 00000", 4);
  statusBar_->insertItem ("Moves: 00000", 3);
  statusBar_->insertItem ("Level: 0000", 2);
  statusBar_->insertItem ("#########################", 1);

  connect (playField_, SIGNAL(levelChanged(const char *)), this, SLOT(changeLevel(const char *)));
  connect (playField_, SIGNAL(collectionChanged(const char *)), this, SLOT(changeCollection(const char *)));
  connect (playField_, SIGNAL(movesChanged(const char *)), this, SLOT(changeMoves(const char *)));
  connect (playField_, SIGNAL(pushesChanged(const char *)), this, SLOT(changePushes(const char *)));

  setStatusBar (statusBar_);
  playField_->emitAll ();

  //resize (playField_->size ());
  //playField_->setSize ();
  updateRects ();
  playField_->setSize ();
  updateRects ();
}

MainWindow::~MainWindow()
{
  delete help_;
  delete animation_;
  delete graphics_;
  delete set_;
  delete game_;
  delete menu_;
  delete playField_;
}



void
MainWindow::focusInEvent (QFocusEvent *) {
  playField_->setFocus ();
}

void
MainWindow::updateGfxMenu (int id) {
  graphics_->setItemChecked (checkedGfx_, false);
  checkedGfx_ = id;
  graphics_->setItemChecked (checkedGfx_, true);
}

void
MainWindow::updateAnimMenu (int id) {
  animation_->setItemChecked (checkedAnim_, false);
  checkedAnim_ = id;
  animation_->setItemChecked (checkedAnim_, true);
}


void
MainWindow::changeGfx (int id) {
  updateGfxMenu (id);
  playField_->resolution (id);
  updateRects ();
}

void
MainWindow::updateSetMenu (int id)
{
  set_->setItemChecked (checkedSet_, false);
  checkedSet_ = id;
  set_->setItemChecked (checkedSet_, true);
}

void
MainWindow::changeCollection (const char *text) {
  statusBar_->changeItem (text, 1);
}

void
MainWindow::changeLevel (const char *text) {
  statusBar_->changeItem (text, 2);
}

void
MainWindow::changeMoves (const char *text) {
  statusBar_->changeItem (text, 3);
}

void
MainWindow::changePushes (const char *text) {
  statusBar_->changeItem (text, 4);
}

