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

#include<B2D.h>

Vector2 Component::Box2DBaseComponent::GetSize(){return Vector2{width, height};}
float Component::Box2DBaseComponent::GetRadius(){return radius;}
float Component::Box2DBaseComponent::GetWidth(){return GetSize().x;}
float Component::Box2DBaseComponent::GetHeight(){return GetSize().y;}
ColliderShape Component::Box2DBaseComponent::GetColliderShape(){return collider_shape;}


/*
  BoxCollider Component
*/
Component::BoxCollider::BoxCollider(float _width, float _height){
  collider_shape = ColliderShape::BOX;
  width = _width;
  height = _height;
  box_collision_shape = new b2PolygonShape();
  box_collision_shape->SetAsBox(width * 0.5f, height * 0.5f);
}


/*
  CircleCollider Component
*/
Component::CircleCollider::CircleCollider(float _radius){
  collider_shape = ColliderShape::CIRCLE;
  radius = _radius;
  circle_collision_shape = new b2CircleShape();
  circle_collision_shape->m_radius = radius;
}


/*
  CapsuleCollider Component
*/
Component::CapsuleCollider::CapsuleCollider(float _height, float _radius){
  collider_shape = ColliderShape::CAPSULE;
  width = _radius * 2;
  height = _height; if(height <= 0) height = 1;
  radius = _radius;
  circle_collision_shape = new b2CircleShape();
  circle_collision_shape->m_radius = radius;
  box_collision_shape = new b2PolygonShape();
  box_collision_shape->SetAsBox(width, height * 0.5f);
}


/*
  EdgeCollider Component
*/
Component::EdgeCollider::EdgeCollider(vector<Vector2> _point_list){
  collider_shape = ColliderShape::EDGE;
  point_list = _point_list;
  polygon_collision_shape = new b2PolygonShape();

  // b2Vec2 vertice[point_list.size()];
  // for(int i = 0; i < point_list.size(); i++){
  //   vertice[i].Set(point_list.at(i).x, point_list.at(i).y);
  // }

  // polygon_collision_shape->Set(vertice, point_list.size());

}

void Component::EdgeCollider::_OnAttach(){
  b2Vec2 vertice[point_list.size()];
  for(int i = 0; i < point_list.size(); i++){
    vertice[i].Set(point_list.at(i).x - node->GetPosition().x,
		   point_list.at(i).y - node->GetPosition().y);
  }

  polygon_collision_shape->Set(vertice, point_list.size());
}


/*
  RigidBody Component
*/
void Component::RigidBody::_OnAttach(){
  b2BodyDef body_def;
  body_def.type = DYNAMIC; 
  body_def.position.Set(node->GetPosition().x, node->GetPosition().y);
  body = world.CreateBody(&body_def);

  if(!_SetCollider()){
    cout<<"you must add collider component at "<<node->name<<" first!"<<endl;
  }
  else{

    if(collider_shape != ColliderShape::CAPSULE){

      fixture_def.density = 1.0f;
      fixture_def.friction = 0.3f;
      fixture = body->CreateFixture(&fixture_def);
      b2Vec2 *_rel_position = new b2Vec2{0, 0};
      fixture->SetUserData(_rel_position);

    } // collider_shape != capsule


    // if(collider_shape == ColliderShape::BOX ||
    //    collider_shape == ColliderShape::CIRCLE){
    //   fixture_def.density = 1.0f;
    //   fixture_def.friction = 0.3f;
    //   fixture = body->CreateFixture(&fixture_def);
    //   b2Vec2 *_rel_position = new b2Vec2{0, 0};
    //   fixture->SetUserData(_rel_position);
    // }
    // else if(collider_shape == ColliderShape::EDGE){}
    // else if(collider_shape == ColliderShape::CAPSULE){}

    body->SetUserData(node);

  }

}

bool Component::RigidBody::_SetCollider(int state){

  switch(state){

  case 0: // box collider

    Node::component_map_it<box_collider> =
      Node::component_map<box_collider>.find(node);

    if(Node::component_map_it<box_collider> !=
       Node::component_map<box_collider>.end()){
      box_collider collider = Node::component_map_it<box_collider>->second;
      fixture_def.shape = collider->box_collision_shape;
      collider_shape = collider->GetColliderShape();
      width = collider->GetWidth();
      height = collider->GetHeight();
      radius = collider->GetRadius();
      delete collider;
      return true;
    }
    else{
      _SetCollider(1);
    }

    break;

  case 1: // circle collider

    Node::component_map_it<circle_collider> =
      Node::component_map<circle_collider>.find(node);

    if(Node::component_map_it<circle_collider> !=
       Node::component_map<circle_collider>.end()){
      circle_collider collider = Node::component_map_it<circle_collider>->second;
      fixture_def.shape = collider->circle_collision_shape;
      collider_shape = collider->GetColliderShape();
      width = collider->GetWidth();
      height = collider->GetHeight();
      radius = collider->GetRadius();
      delete collider;
      return true;
    }
    else{
      _SetCollider(2);
    }

    break;

  case 2: // capsule collider

    Node::component_map_it<capsule_collider> =
      Node::component_map<capsule_collider>.find(node);

    if(Node::component_map_it<capsule_collider> !=
       Node::component_map<capsule_collider>.end()){

      capsule_collider collider =
	Node::component_map_it<capsule_collider>->second;

      collider_shape = collider->GetColliderShape();
      width = collider->GetWidth();
      height = collider->GetHeight();
      radius = collider->GetRadius();

      // b2FixtureDef circle_fixture_def;

      fixture_def.shape = collider->box_collision_shape;

      fixture_def.density = 1.0f;
      fixture_def.friction = 0.3f;

      // upper circle
      collider->circle_collision_shape->m_p = b2Vec2{0, -height / 2};
      body->CreateFixture(collider->circle_collision_shape, 1.0f);
      
      // mid box
      body->CreateFixture(&fixture_def);

      // bottom circle
      collider->circle_collision_shape->m_p = b2Vec2{0, height / 2};
      body->CreateFixture(collider->circle_collision_shape, 1.0f);

      fixture = body->GetFixtureList();

      int count = 0;
      for(fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){
	if(fixture->GetType() == 2){
	  fixture->SetUserData(new b2Vec2{0, 0});
	  count++;
	}
	else{
	  if(count == 0){
	    b2Vec2 *_rel_position = new b2Vec2{0, -height / 2};
	    fixture->SetUserData(_rel_position);
	    count++;
	  }
	  else{
	    b2Vec2 *_rel_position = new b2Vec2{0, height / 2};
	    fixture->SetUserData(_rel_position);
	    count++;
	  }
	}
      }

      delete collider;
      return true;
    }
    else{
      _SetCollider(3);
    }

    break;

  case 3: // edge collider

    Node::component_map_it<edge_collider> =
      Node::component_map<edge_collider>.find(node);

    if(Node::component_map_it<edge_collider> !=
       Node::component_map<edge_collider>.end()){
      edge_collider collider = Node::component_map_it<edge_collider>->second;
      fixture_def.shape = collider->polygon_collision_shape;
      collider_shape = collider->GetColliderShape();
      delete collider;
      return true;
    }
    else{
      return false;
    }
    break;

    return false;

  } // switch

}

void Component::RigidBody::Step(){
  node->SetPosition(Vector2{body->GetPosition().x, body->GetPosition().y});
  node->SetRotation(Rad2Deg(body->GetAngle()));
}

b2Vec2 Component::RigidBody::GetBodyPosition(){return body->GetPosition();}
float Component::RigidBody::GetBodyRadian(){return body->GetAngle();}
void Component::RigidBody::SetBodyType(b2BodyType type){body->SetType(type);}
void Component::RigidBody::SetFixedRotation(bool is_fixed){body->SetFixedRotation(is_fixed);}
b2Body* Component::RigidBody::GetBody(){return body;}


void B2D::SetContactListener(b2ContactListener *contact_listener){
  world.SetContactListener(contact_listener);
}

void B2D::Step(){
  world.Step(time_step, velocity_iterations, position_iterations); 
  
  for(b2Body *body = world.GetBodyList(); body; body = body->GetNext()){
    static_cast<Node*>(body->GetUserData())->GetComponent<Component::rigid_body>()->Step();
  }
}

void B2D::DebugDraw(float opacity , Color color1, Color color2){
  if(opacity > 1) opacity = 1;
  for(b2Body *body = world.GetBodyList(); body; body = body->GetNext()){
    Color curr_color = color1;
    Node *node = static_cast<Node*>(body->GetUserData());

    curr_color.a *= opacity;

    for(b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){

      b2Vec2 *rel_position = static_cast<b2Vec2*>(fixture->GetUserData());

      Vector2 draw_position = TransformRotation(body->GetAngle(),
						Vector2{body->GetPosition().x + rel_position->x, body->GetPosition().y + rel_position->y},
						Vector2{body->GetPosition().x, body->GetPosition().y}
						);

      switch(fixture->GetType()){
      case 0: // circle shape

	DrawCircle(draw_position.x,
		   draw_position.y,
		   fixture->GetShape()->m_radius,
		   curr_color);     
	break;

      case 1: // edge shape
	break;

      case 2: // rectangle shape
	DrawRectangle(draw_position.x,
		      draw_position.y,
		      node->GetComponent<Component::rigid_body>()->GetSize().x,
		      node->GetComponent<Component::rigid_body>()->GetSize().y,
		      curr_color,
		      Rad2Deg(body->GetAngle()));
	break;

      case 4:
	break;

      } // switch

    } // for fixture

  } // for body

}
