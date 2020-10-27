/*
  license under zlib license
  Copyright (C) 2020 Kevin Haryo Kuncoro

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source distribution.

  Kevin Haryo Kuncoro
  kevinhyokun91@gmail.com
*/

#ifndef N_COMPONENT_DERIVED_CHECK
#define N_COMPONENT_DERIVED_CHECK

#include<iostream>

namespace Component{

typedef struct BaseComponent n_base;
typedef struct DrawableBaseComponent n_drawable;
typedef struct RigidBody n_rigidbody;

 template<typename T>
   static bool IsDerivedComponent(T component){
   if(std::is_base_of<BaseComponent, T>::value == 1){
     return true; 
   }
   return false;
 }

 template<typename T>
   static bool IsDerivedDrawable(T component){
   if(std::is_base_of<DrawableBaseComponent, T>::value == 1){
     return true; 
   }
   return false;
 }

 template<typename T>
   static bool IsDerivedRigidBody(T component){
   if(std::is_base_of<RigidBody, T>::value == 1){
     return true; 
   }
   return false;
 }

}

#endif