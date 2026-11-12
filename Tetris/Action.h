#pragma once

enum class Action { None, Left, Right, Down, Rotate, Quit };

Action ReadAction();